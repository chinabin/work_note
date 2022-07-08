#!/bin/env python3
# coding:utf-8

import os
import sys
import datetime

def load_cfg(cfg_file):
    i = 0
    c = 0
    s = 0
    k = 0
    li = []

    with open(cfg_file) as f:
        for l in f:
            if i == 0:
                t1, _, _, _, _, _, _ = l.split(' ')
                t1 = t1.split('-')[0]
                t1 = t1[1:]
                # print(t1)
                t1 = datetime.datetime.strptime(t1, '%H:%M:%S.%f')
            else:
                c = c + 1
                t2, _, _, _, _, _, _ = l.split(' ')
                t2 = t2.split('-')[0]
                t2 = t2[1:]
                # print(t2)
                t2 = datetime.datetime.strptime(t2, '%H:%M:%S.%f')

                lala = (t2 - t1).microseconds / 1000
                if lala < 10:
                    print(c)
                li.append(lala)
                s = s + lala

            i = 1 if i == 0 else 0
    print(c, s, s / c)
    li.sort()
    c = 0
    for d in li:
        c = c + 1
        if c == len(li) / 2:
            print(d)
    print(li)




def main():
    load_cfg("123")

if __name__ == "__main__":
    main()
