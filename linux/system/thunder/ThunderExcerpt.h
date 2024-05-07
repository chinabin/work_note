//
// Created by alfred on 17-1-13.
//

#ifndef THUNDER_THUNDEREXCERPT_H
#define THUNDER_THUNDEREXCERPT_H

#include <cstring>
#include <iostream>

namespace thunder {


    /*
     * 数据存储到内存，是没有类型信息的，所以需要一个办法：既有数据，又有类型

     * pContent 存储格式化后的数据， length 是其当前长度
     * message, ptrContent 是当前存储的数据长度
     * format 是格式控制， ptrFormat 是目前存储的值或者格式的个数
     * 
     * write:
     *      int, i
     *      unsigned int, I
     *      unsigned int, I
     *      long long int int, q
     *      unsigned long long, Q
     *      long, l
     *      double, d
     *      float, f
     *      string, s
     *      bool, ?
     *      char, c
    */

    class ThunderExcerpt {
    public:
        ThunderExcerpt(char* content):pContent(content) {
            clear();
        }

        ~ThunderExcerpt() {

        }

        void updatePtr(char* content) {
            pContent = content;
        }

        void clear() {
            ptrContent = ptrFormat = length = 0;
            memset(&message, 0, 512);
            memset(&format, 0, 128);
        }

        // 将长度为 length 的 value 放入 message 
        inline void contentFill(void* value, int length) {
            memcpy((message + ptrContent), value, length);
            ptrContent += length;
        }


        template <typename iType>
        ThunderExcerpt& operator<<(const iType &value) {
            return write(value);
        }

        ThunderExcerpt& operator<<(ThunderExcerpt& (*_Pfn)(ThunderExcerpt&)) {
            return _Pfn(*this);
        }

        template <class Arg>
        ThunderExcerpt &log(const Arg &value) {
            write(value);
            return *this;
        }

        template <class Arg, class ...Args>
        ThunderExcerpt& log(const Arg &value, const Args& ...args) {
            log(value);
            log(args...);
            end();
            return *this;
        }

        ThunderExcerpt& write(const int& value) {
            format[ptrFormat++] = 'i';

            contentFill((void*)&value, sizeof(int));

            return (*this);
        }

        ThunderExcerpt& write(const unsigned int& value) {
            format[ptrFormat++] = 'I';

            contentFill((void*)&value, sizeof(unsigned int));

            return (*this);
        }

        ThunderExcerpt& write(const long long int& value) {
            format[ptrFormat++] = 'q';

            contentFill((void*)&value, sizeof(long long int));
            return (*this);
        }

        ThunderExcerpt& write(const unsigned long long& value) {
            format[ptrFormat++] = 'Q';

            contentFill((void*)&value, sizeof(unsigned long long));
            return (*this);
        }

        ThunderExcerpt& write(const unsigned long int& value) {
            format[ptrFormat++] = 'Q';

            contentFill((void*)&value, sizeof(unsigned long int));
            return (*this);
        }

        ThunderExcerpt& write(const long& value) {
            format[ptrFormat++] = 'l';

            contentFill((void*)&value, sizeof(long));
            return (*this);
        }

        ThunderExcerpt& write(const double& value) {
            format[ptrFormat++] = 'd';

            contentFill((void*)&value, sizeof(double));
            return (*this);
        }

        ThunderExcerpt& write(const float& value) {
            format[ptrFormat++] = 'f';

            contentFill((void*)&value, sizeof(float));
            return (*this);
        }

        ThunderExcerpt& write(const char* value) {
            short len = strlen(value)+1;

            format[ptrFormat++] = 's';
            memcpy(format+ptrFormat, &len, sizeof(short));
            ptrFormat += sizeof(short);

            contentFill((void*)value, len);
            message[ptrContent-1] = 0;
            return (*this);
        }

        ThunderExcerpt& write(const std::string& value) {
            return write(value.c_str());
        }

        ThunderExcerpt& write(const bool& value) {
            format[ptrFormat++] = '?';

            contentFill((void*)&value, sizeof(bool));
            return (*this);
        }

        ThunderExcerpt& write(const char& value) {
            format[ptrFormat++] = 'c';

            contentFill((void*)&value, sizeof(char));
            return (*this);
        }

        /*
         * F + format num + format + M + data length + data
        */
        int end() {
            int ptr = 0;
            pContent[ptr++] = 'F';
            memcpy(pContent + ptr, &ptrFormat, sizeof(short));
            ptr +=sizeof(short);
            memcpy(pContent + ptr, format, ptrFormat);
            ptr += ptrFormat;
            pContent[ptr++] = 'M';
            memcpy(pContent + ptr, &ptrContent, sizeof(short));
            ptr += sizeof(short);

            memcpy(pContent+ ptr, message, ptrContent);
            ptr += ptrContent;

            length = ptr;
            return ptr;
        }


        std::string str(char split = ' ') {
            std::ostringstream oss;

            if (pContent == nullptr) {
                return "";
            }

            if (pContent[0] != 'F') {
                return "";
            }

            short lenHeader;
            memcpy(&lenHeader, pContent + 1, sizeof(short));

            if (pContent[1+sizeof(short)+ lenHeader] != 'M') {
                return "";
            }

            short lenMsg;
            memcpy(&lenMsg, pContent+ 1 + sizeof(short) + lenHeader + 1, sizeof(short));

            char* pHeader = pContent + 1 + sizeof(short);
            char* pMsg = (pContent + 1 + sizeof(short) + lenHeader + 1 + sizeof(short));

            for (int i = 0; i < lenHeader; i ++ ) {
                switch( *(pHeader + i )) {
                    case 'i':
                    {
                        int val;
                        memcpy(&val, pMsg, sizeof(int));

                        oss << val << split;
                        pMsg += sizeof(int);
                    }

                        break;
                    case 'f':
                    {
                        float val;
                        memcpy(&val, pMsg, sizeof(float));

                        oss << val << split;
                        pMsg += sizeof(float);
                    }
                        break;
                    case 'd':
                    {
                        double val;
                        memcpy(&val, pMsg, sizeof(double));

                        oss << val << split;
                        pMsg += sizeof(double);
                    }
                        break;
                    case 'l':
                    {
                        long val;
                        memcpy(&val, pMsg, sizeof(long));

                        oss << val << split;
                        pMsg += sizeof(long);
                    }
                        break;
                    case 'q':
                    {
                        long long int val;
                        memcpy(&val, pMsg, sizeof(long long int));

                        oss << val << split;
                        pMsg += sizeof(long long int);
                    }
                        break;
                    case 'Q':
                    {
                        unsigned long long val;
                        memcpy(&val, pMsg, sizeof(unsigned long long));

                        oss << val << split;
                        pMsg += sizeof(unsigned long long);
                    }
                        break;
                    case 'I':
                    {
                        unsigned int val;
                        memcpy(&val, pMsg, sizeof(unsigned int));

                        oss << val << split;
                        pMsg += sizeof(unsigned int);
                    }
                        break;
                    case 'c':
                    {
                        char *val = (char*)pMsg;

                        oss << (*val) << split;
                        pMsg += sizeof(char);
                    }
                        break;
                    case '?':
                    {
                        bool *val = (bool*)pMsg;

                        oss << (*val) << split;
                        pMsg += sizeof(bool);
                    }
                        break;
                    case 's':
                    {
                        char* val = pMsg;
                        short* len = (short*)(pHeader + i + 1);
                        oss << val << split;

                        pMsg += (*len);
                        i += sizeof(short);
                    }
                        break;
                    default:
                        break;
                }

            }

            return (oss.str());
        }

        void print(char split = ' ') {

            std::cout << str(split) << std::endl;

        }

        short len() {
            return length;
        }


    private:
        char*   pContent;
        char    format[128];
        char    message[512];

        short     ptrContent = 0, ptrFormat = 0;
        short length;
    };



    // manipulators

    inline ThunderExcerpt& endlog(ThunderExcerpt& __excerpt) {
        __excerpt.end();
        return __excerpt;
    }
}


#endif //THUNDER_THUNDEREXCERPT_H
