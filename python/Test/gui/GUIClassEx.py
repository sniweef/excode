'''
Created on 2012-12-11

@author: hzhigeng
'''
from tkinter import *
from tkinter.messagebox import showinfo

class GUIEx(Frame):
    def __init__(self, parent=None):
        Frame.__init__(self, parent)
        Button(self, text='button', command=self.reply).pack()
    def reply(self):
        showinfo(title='popup', message='button pressed')

if __name__ == '__main__':
    mainwin = Tk()
    Label(mainwin, text=__name__).pack()

    popup = Toplevel()
    Label(popup, text='attach').pack(side=LEFT)
    
    GUIEx(popup).pack(side=RIGHT)
    mainwin.mainloop()