'''
Created on 2012-12-11

@author: hzhigeng
'''
if __name__ == '__main__':
    from Database import db, printdb  
    import pickle
    dbfile = open('dbfile', 'wb')
    pickle.dump(db, dbfile)
    dbfile.close()
    
    dbfile = open('dbfile', 'rb')
    newdb = pickle.load(dbfile)
    print('newdb:')
    printdb(newdb)
    dbfile.close()
    
    