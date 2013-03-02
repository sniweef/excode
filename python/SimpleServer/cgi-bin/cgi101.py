#!/usr/bin/python3
'''
Created on 2012-12-11

@author: hzhigeng
'''
import cgi, sys
form = cgi.FieldStorage()
print('Content-type: text/html\n\n')
print('<title>Reply</title>')
if not 'user' in form:
    print('<h1>Who are you?</h1>')
else:
    print('<h1>Hello <i>%s</i>!</h1>'%cgi.escape(form['user'].value))
