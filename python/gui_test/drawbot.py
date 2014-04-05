#!/usr/bin/python
# -*- coding: utf-8 -*-

from Tkinter import *

WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600

class Example(Frame):
  
    def __init__(self, parent):
        Frame.__init__(self, parent)   
         
        self.parent = parent        
        self.initUI()
        
    def initUI(self):
        self.parent.title("Drawbot")


def main():
    
    root = Tk()
    ex = Example(root)

    pos_x = (root.winfo_screenwidth()/2) - (WINDOW_WIDTH/2)
    pos_y = (root.winfo_screenheight()/2) - (WINDOW_HEIGHT/2)
    root.geometry('%dx%d+%d+%d' % (WINDOW_WIDTH, WINDOW_HEIGHT, pos_x, pos_y))

    root.mainloop()

if __name__ == '__main__':
    main()  