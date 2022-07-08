#!/bin/env python3
# coding:utf-8

import os
import sys
import datetime

def load_cfg(cfg_file):
    ret = dict()
    uu = 0

    with open(cfg_file) as f:
        for l in f:
            y, t, _, _, m1 = l.split(' ')
            t = y[1:] + ' ' + t[:-1]

            u,_,o,_ = m1.split(',', 3)
            u = u.split(':')[1]
            o = o.split(':')[1]

            d = dict()
            d[o] = t

            ret[u] = []

    with open(cfg_file) as f:
        for l in f:
            y, t, _, _, m1 = l.split(' ')
            t = y[1:] + ' ' + t[:-1]

            u,_,o,_ = m1.split(',', 3)
            u = u.split(':')[1]
            o = o.split(':')[1]

            d = dict()
            d[o] = datetime.datetime.strptime(t, '%Y-%m-%d %H:%M:%S.%f')

            ret[u].append(d)

    return ret

    




def main():
    # req = load_cfg("OrderInputReq_yd3")
    req = load_cfg("OrderInputReq_bl2")
    # req = load_cfg("aa")
    # input_rsp = load_cfg("OrderInputRsp")
    # input_rsp = load_cfg("bb")
    # action_rsp = load_cfg("OrderActionRsp_yd3")
    action_rsp = load_cfg("OrderActionRsp_bl2")
    # d1 = datetime.datetime.strptime('2022-04-26 09:30:07.635731', '%Y-%m-%d %H:%M:%S.%f')
    # for u in req:
    #     print(u)
    #     for it in req[u]:
    #         for i1 in it:
    #             print('    ', (it[i1] - d1).seconds, (it[i1] - d1).microseconds)
    
    gap = 10
    c = 0
    s = 0.0
    cc = 0
    for u1 in req:
        for u2 in action_rsp:
            # if u1 == u2 and u1 == '2116035':
            # if u1 == u2 and u1 == '2120212':
            if u1 == u2:
                for it1 in req[u1]:
                    for o1 in it1:
                        for it2 in action_rsp[u2]:
                            if o1 in it2:
                                c = c + 1
                                lala = (it2[o1] - it1[o1]).microseconds / 1000
                                # print(lala)
                                if lala > gap:
                                    # print(c)
                                    cc = cc + 1
                                    # print(o1, it2[o1], it1[o1])
                                s = s + lala
                                break

        # for u2 in action_rsp:
        #     if u1 == u2:
        #         for it1 in req[u1]:
        #             for o1 in it1:
        #                 for it2 in input_rsp[u2]:
        #                     if o1 in it2:
        #                         c = c + 1
        #                         lala = s + (it2[o1] - it1[o1]).microseconds / 1000
        #                         s = s + lala
        #                         if lala > gap:
        #                             tt = tt + 1
        #                             print(lala)
                                # print('    ', (it2[o1] - it1[o1]).microseconds / 1000)

    print(cc, c, s, s / c)

if __name__ == "__main__":
    main()
