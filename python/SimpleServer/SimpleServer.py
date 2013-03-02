#!/usr/bin/python3
'''
Created on Dec 19, 2012

@author: sniweef
'''

import os
from http.server import HTTPServer, CGIHTTPRequestHandler

webdir='/home/sniweef/programming/python/SimpleServer'
port=80

os.chdir(webdir)
srvraddr=('sniweef-pc',port)
srvrobj=HTTPServer(srvraddr,CGIHTTPRequestHandler)
print('serving...')
srvrobj.serve_forever()


if __name__ == '__main__':
    pass
