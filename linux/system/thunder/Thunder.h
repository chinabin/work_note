//
// Created by alfred on 17-1-8.
//

#ifndef THUNDER_THUNDER_H
#define THUNDER_THUNDER_H

#include <atomic>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <string>
#include "thunderRingBuffer.h"
#include "thunderMacros.h"
#include "thunderhelpers.h"
#include "ThunderExcerpt.h"

#ifndef THUNDER_CONFIGURE_BUFFER_LENGTH
  #define THUNDER_CONFIGURE_BUFFER_LENGTH 512
#endif

#define CONTENT_LENGTH (THUNDER_CONFIGURE_BUFFER_LENGTH-sizeof(short)*4-sizeof(size_t))

#define CURRENT_CORE_EXCEPT thunder::Thunder::get()->excerpt()
#define CURRENT_CORE_PUB_EXCEPT_CONSOLE thunder::Thunder::get()->pubExcerpt(thunder::CONSOLE, thunder::DEBUG);
#define CURRENT_CORE_PUB_EXCEPT_FILE thunder::Thunder::get()->pubExcerpt(thunder::FILE, thunder::DEBUG);
#define ENDLOG thunder::endlog

namespace thunder {

    enum SinkType { // not mutually exclusive
        CONSOLE = 1,
        FILE = 2,
        UDP = 4,
        TCP = 8,
    };

    enum Level {
        INFO = 0,
        DEBUG= 1,
        WARN = 2,
        ERROR = 3,
    };

    class Thunder {
    public:


#pragma pack(push)
#pragma pack(1)

    struct logEntryHead {
        short entryFlag;  // Reference Flag Table
        size_t localTS; // Log Time
        short sinkType;  // Console, File, UDP, ...
        short level;  // WARN, ERROR, INFO, ...
        short length;
    };

    struct logEntry {
        short entryFlag;  // Reference Flag Table
        size_t localTS; // Log Time
        short sinkType;  // Console, File, UDP, ...
        short level;  // WARN, ERROR, INFO, ...
        short length;
        char content[CONTENT_LENGTH];
    };
#pragma pack(pop)

        typedef thunderRingBufferT<logEntry> logBufferType;

        // prefault: populate(prefault) page tables for mapping
        Thunder(const char* shmName, size_t bufferSize, bool prefault = false)
            :logRingBuffer(shmName, true, bufferSize, prefault) {
            if (!logRingBuffer.isIsValid()) {
                std::cerr << "Thunder Log at shm: " << shmName
                          << " initializing failed..."<< std::endl;

                isEnabled = false;
            }

            updateExcerpt();
        }

        // initialize by core
        Thunder(const int core_id, size_t bufferSize, bool prefault = false):
            Thunder((std::string("thunder-") + std::to_string(core_id)).c_str(), bufferSize, prefault) {
            (instance())[core_id] = this;
        }


        ~Thunder() {
            if (pExcerpt) {
                delete pExcerpt;
                pExcerpt = nullptr;
            }

        }

        ThunderExcerpt& excerpt() {
            assert(pExcerpt != nullptr);
            return (*pExcerpt);
        }

        void updateExcerpt() {
            if (pExcerpt == nullptr) {
                pExcerpt = new ThunderExcerpt(logRingBuffer.getCurrentNode()->data.content);
            } else {
                pExcerpt->clear();
                pExcerpt->updatePtr(logRingBuffer.getCurrentNode()->data.content);
            }

        }

        bool log(short entryFlag, const short sinkType,
                short level, short length, const char* content) {

            if (!isEnabled) {
                return false;
            }

            if (length > (int) CONTENT_LENGTH) {
                std::cerr << "Log content length overflow..." << std::endl;
                return false;
            }

            logBufferType::node* n = logRingBuffer.getCurrentNode();

            n->data.entryFlag = entryFlag;
            n->data.length = length;
            n->data.level = level;
            n->data.sinkType = sinkType;
            n->data.localTS = getHighResTimeRT_US();

            memcpy( &(n->data.content), content, length);

            logRingBuffer.moveNext();

            updateExcerpt();

            return true;
        }

        bool info(short entryFlag, const short sinkType,
            short length, const char* content) {
            return log(entryFlag, sinkType, INFO, length, content);
        }

        bool error(short entryFlag, const short sinkType,
                  short length, const char* content) {
            return log(entryFlag, sinkType, ERROR, length, content);
        }

        bool warn(short entryFlag, const short sinkType,
                  short length, const char* content) {
            return log(entryFlag, sinkType, WARN, length, content);
        }

        bool debug(short entryFlag, const short sinkType,
                  short length, const char* content) {
            return log(entryFlag, sinkType, DEBUG, length, content);
        }

        void* claimNext() {
            logBufferType::node* n = logRingBuffer.getCurrentNode();

            return (void*)(n->data.content);
        }

        bool publish(short entryFlag, const short sinkType,
                     short level, short length) {
            if (!isEnabled) {
                return false;
            }
            logBufferType::node* n = logRingBuffer.getCurrentNode();

            n->data.entryFlag = entryFlag;
            n->data.length = length;
            n->data.level = level;
            n->data.sinkType = sinkType;
            n->data.localTS = getHighResTimeRT_US();

            std::atomic_thread_fence(std::memory_order_release);
            logRingBuffer.moveNext();
            updateExcerpt();
            return true;
        }

        bool pubExcerpt(const short sinkType, short level) {
            if (!isEnabled) {
                return false;
            }

            if (pExcerpt->len() == 0) {
                return false;
            }
            logBufferType::node* n = logRingBuffer.getCurrentNode();

            n->data.entryFlag = (short)EntryFlag ::EXCERPT;
            n->data.length = pExcerpt->len();
            n->data.level = level;
            n->data.sinkType = sinkType;
            n->data.localTS = getHighResTimeRT_US();

            logRingBuffer.moveNext();
            updateExcerpt();
            return true;
        }


        void reset() {
            logRingBuffer.resetCount();

            updateExcerpt();
        }

        static Thunder** instance() {
            static Thunder* coreMap[64];

            return coreMap;
        }

        static Thunder* get(const std::string& shmName) {
            static std::unordered_map<std::string, Thunder*> _thunder_name;
            auto it = _thunder_name.find(shmName);
            if(it != _thunder_name.end()){
                return it->second;
            }
            else {
                _thunder_name[shmName] = new Thunder(shmName.c_str(), SHM_RB_SIZE_T);
            }

            return _thunder_name[shmName];
        }


        static Thunder* get(int core) {
            if (instance()[core] == nullptr) {
                instance()[core] = new Thunder(core, SHM_RB_SIZE_T, true);
            }
            return (instance())[core];
        }

        static Thunder* get() {
            return get(sched_getcpu());
        }

        // pass pointer to member function to handler
        template <class RetT, class ClassT, class ArgT>
        static void pollEntry(logBufferType *reader, ClassT *obj,
                              RetT (ClassT::*handler)(const ArgT *),
                              EntryFlag flag, unsigned long &last_pos) {
            auto curr_pos = reader->getCount();
            if (curr_pos > last_pos) {
                for (auto i = last_pos; i < curr_pos; ++i) {
                    auto &entry = reader->getOneNode(i)->data;
                    if (thunder::EntryFlag(entry.entryFlag) == flag) {
                        auto *data = (ArgT*) reader->getOneNode(i)->data.content;
                        (obj->*handler)(data);
                    }
                }
                last_pos = curr_pos;
            }
            else if (curr_pos < last_pos) {
                last_pos = curr_pos;
            }
        }

    private:

        size_t getHighResTimeRT()
        {
            struct timespec tp;

            clock_gettime(CLOCK_REALTIME, &tp);

            return ((unsigned long long)tp.tv_sec * 1000000000+ tp.tv_nsec);
        }

        size_t getHighResTimeRT_US()
        {
            struct timespec tp;

            clock_gettime(CLOCK_REALTIME, &tp);

            return ((unsigned long long)tp.tv_sec * 1000000+ tp.tv_nsec / 1000);
        }


        logBufferType logRingBuffer;
        bool isEnabled = true;

        ThunderExcerpt* pExcerpt = nullptr;


    };


}

#endif //THUNDER_THUNDER_H
