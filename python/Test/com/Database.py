'''
Created on 2012-12-11

@author: hzhigeng
'''
def printdb(db):
    for dbkey in db:
        print(dbkey, '=>', db[dbkey])

hx = {'name':'hx', 'age':20, 'pay':5000}
gj = {'name':'gj', 'age':22, 'pay':5500}
lj = {'name':'lj', 'age':19, 'pay':5300}

db = {'hx':hx, 'gj':gj, 'lj':lj}

if __name__ == '__main__':
    for dbkey in db:
        print(dbkey, '=>', db[dbkey])