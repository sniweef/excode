import sys

if sys.argv[1]:
    file = open(sys.argv[1], 'r+')
    file.write('a')
    file.close()
