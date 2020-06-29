#WireFrame of hand
import math
from tkinter import *
 
global x1,x2,x3,x4
x0 = 140
x1 = 180
x2 = 220
x3 = 260
x4 = 100

def computeR(mi,ma,cur):
    r = (1-(ma - cur)/(ma-mi))
    return r

def fingerOne(mi,ma,cur,y):
    r0 = computeR(mi[0],ma[0],cur[0])
    r1 = computeR(mi[1],ma[1],cur[1])
    r2 = computeR(mi[2],ma[2],cur[2])
    y1 = y-(r0*80)
    y2 = y1-(r1*80)
    y3 = y2-(r2*80)
    return [y3,y2,y1]

def thumbFing(mi,ma,cur,y):
    r0 = computeR(mi[0],ma[0],cur[0])
    r1 = computeR(mi[1],ma[1],cur[1])
    y1 = y-(r0*80)
    y2 = y1-(r1*80)
    return [y2,y1]

window = Tk()
window.title("wireflame")

c = Canvas(window, bg = "white", height = 500, width = 500)

y = 300
y2 = 350

print(fingerOne(12,55,35,y))

[f1y3,f1y2,f1y1] = fingerOne(12,55,35,y)
[f2y3,f2y2,f2y1] = fingerOne(12,55,35,y)
[f3y3,f3y2,f3y1] = fingerOne(12,55,35,y)
[f4y3,f4y2,f4y1] = fingerOne(12,55,35,y)
[th2,th1] = thumbFing(12,55,35,y2)

c.create_line(x0,y,x0,f1y1,fill ="red")
c.create_line(x0,f1y1,x0,f1y2,fill ="blue")
c.create_line(x0,f1y2,x0,f1y3,fill ="green")

c.create_line(x1,y,x1,f2y1,fill ="red")
c.create_line(x1,f2y1,x1,f2y2,fill ="blue")
c.create_line(x1,f2y2,x1,f2y3,fill ="green")

c.create_line(x2,y,x2,f3y1,fill ="red")
c.create_line(x2,f3y1,x2,f3y2,fill ="blue")
c.create_line(x2,f3y2,x2,f3y3,fill ="green")

c.create_line(x3,y,x3,f3y1,fill ="red")
c.create_line(x3,f3y1,x3,f3y2,fill ="blue")
c.create_line(x3,f3y2,x3,f3y3,fill ="green")

c.create_line(x4,y2,x4,th1,fill ="red")
c.create_line(x4,th1,x4,th2,fill ="blue")

#c.create_oval(x4-5,y2+150,x3+30,y-30)

c.pack()
window.mainloop()
