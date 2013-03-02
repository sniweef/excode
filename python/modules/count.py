#!/usr/bin/env python2

def countLines(name):
    try :
        length = len(open(name).readlines())
    except :
        print 'open file error!',
        length = -1
    else :
        return length

def countChar(name):
    try :
        s = open(name).read()
        n = len(s)
    except :
        n = -1
    else :
        return n

def test(name):
        print name + ' : ',
        print str(countLines(name)) + ' lines',
        print str(countChar(name)) + ' chars'

if __name__ == '__main__':
    import sys
    for each in sys.argv:
        test(each)
