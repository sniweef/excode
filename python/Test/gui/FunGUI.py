'''
Created on 2012-12-11

@author: hzhigeng
'''

from tkinter import *
import random

fontsize = 30
colors = ['red', 'green', 'blue', 'yellow', 'orange', 'cyan']

def onSpam():
    popup = Toplevel()
    color = random.choice(colors)
    Label(popup, text='popup', bg='black', fg=color).pack(fill=BOTH)
    mainLabel.config(fg=color)

def onFlip():
    mainLabel.config(fg=random.choice(colors))
    main.after(250, onFlip)
    
def onGrow():
    global fontsize
    fontsize += 5
    mainLabel.config(font=('arial', fontsize, 'italic'))
    main.after(100, onGrow)
    
main = Tk()
main.title('FunGui')
mainLabel = Label(main, text='FunGui', relief=RAISED)
mainLabel.config(font=('arial', fontsize, 'italic'), fg='cyan', bg='navy')
mainLabel.pack(side=TOP, expand=YES, fill=BOTH)
Button(main, text='spam', command=onSpam).pack(fill=X)
Button(main, text='flip', command=onFlip).pack(fill=X)
Button(main, text='grow', command=onGrow).pack(fill=X)
main.mainloop()
