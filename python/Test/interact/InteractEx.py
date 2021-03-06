'''
Created on 2012-12-11

@author: hzhigeng
'''

import shelve
fieldnames = ('name', 'age', 'pay')
maxlen = max(len(f) for f in fieldnames)
db = shelve.open('shelvedb')
#from com.Database import printdb
#printdb(db)

while True:
    key = input('key:')
    try:
        record = db[key]
    except:
        print('No such key:%s'%key)
    else:
        for field in fieldnames:
            print(field.ljust(maxlen), '=>', record[field])               

if __name__ == '__main__':
    pass