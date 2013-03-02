#!/usr/bin/env python2
import sys
dirname = sys.argv[1]
import os, glob
allsizes = []
allpy = glob.glob(os.path.join(dirname, '*.py'))
for filename in allpy:
    filesize = os.path.getsize(filename)
    allsizes.append((filesize, filename))
    allsizes.sort()
print allsizes[:2]
print allsizes[-2:]
