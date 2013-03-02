#!/usr/bin/env python3

import _thread

def child(tid):
    print("Hello child : ", tid)

def parent():
    i = 0
    while True:
        i += 1
        _thread.start_new_thread(child, (i,))
        if input() == 'q': break

parent()
