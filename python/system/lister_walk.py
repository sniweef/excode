#!/usr/bin/env python3

import sys, os

def lister(root):
    for (thisdir, subdir, files) in os.walk(root):
        print('[' + thisdir + ']')
        for fname in files:
            path = os.path.join(thisdir, fname)
            print(path)

if __name__ == '__main__':
    lister(sys.argv[1])
