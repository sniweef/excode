#!/usr/bin/env python3
import os

def child():
    print("Child pid:", os.getpid())
    os._exit(0)

def parent():
    while True:
        newpid = os.fork()
        if newpid == 0:
            child()
        else:
            print("Parent pid:", os.getpid())

        if input() == 'q':
            break

parent()
