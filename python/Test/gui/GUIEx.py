'''
Created on 2012-12-11

@author: hzhigeng
'''

from tkinter.messagebox import showinfo

def reply():
    showinfo(title='popup', message='Button pressed')


if __name__ == '__main__':
    from tkinter import *
    window = Tk()
    button = Button(window, text='button', command=reply)
    button.pack()
    window.mainloop();