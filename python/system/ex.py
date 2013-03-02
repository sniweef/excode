#!/usr/bin/env python3

def scanner(name, function):
    #for line in open(name, 'r')
    #    function(line)
    list(map(function, open(name, 'r')))

from sys import argv
class UnkownCommand(Exception):
    pass

def processLine(line):
    if line[0] == '*':
        print("Ms.", line[1:-1])
    elif line[0] == '+':
        print("Mr.", line[1:-1])
    else:
        raise UnkownCommand(line)

filename = 'data.txt'
if len(argv) == 2:
    filename = argv[1]
scanner(filename, processLine)
