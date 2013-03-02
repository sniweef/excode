'''
Created on 2012-12-11

@author: hzhigeng
'''

if __name__ == '__main__':
    from com.Database import gj, lj, hx, printdb
    import shelve
    db = shelve.open('shelvedb')
    db['gj'] = gj
    db['lj'] = lj
    db['hx'] = hx
    db.close()
    
    newdb = shelve.open('shelvedb')
    newgj = newdb['gj']
    newgj['age'] = 30
    newdb['gj'] = newgj
    print('shelvedb:')
    printdb(newdb)
    newdb.close()
        