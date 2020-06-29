from tkinter import *
import tkinter.messagebox
import tkinter as tk
import time
from threading import Timer

def doNothing():
    y=10+5

def doNothing2(gyro):
    if (gyro=='T'):
        print('1')
    else:
        print('0')


#********Basic Functions ********
def okmessage(title, mess):
    tkinter.messagebox.showinfo(title, mess)

def mul_question (title, mess):
    answer = tkinter.messagebox.askquestion(title, mess)
    if answer == 'redo':
        return 1
    elif (answer == 'move on'):
        return 2
    else:
        return 3
    
def yes_question (title, mess):
    answer = tkinter.messagebox.askquestion(title, mess)
    if answer == 'yes':
        return True
    else:
        return False

    
#**********Calibration Functions**********

#***Gyroscope Cali *****
gyro_on=False

def gyro ():
    print("Gyro activated")
    okmessage ("Step 1", "Prepare to hold your hand parallel to floor for 5s")
    okmessage ("Step 2", "Data collections after 5s")
    # app = ExampleApp()
    global gyro_on
    gyro_on = True
    print(str(gyro_on)+ "activated")
    print(str(gyro_getter()))
    
    with open('state_data.txt', 'w') as file:
        file.writelines("True")
    file.close()

def gyro_getter():
    return gyro_on

def gyro_changer():
    print("it is changed")
    gyro_on = False
    cali_done()
    # time.sleep(20)
    # print("Gyro_on is "+str(gyro_on))
    # gyro_on = False
    #cali_done()
    
def cali_redo_1():
    okmessage ("Step 1", "Prepare to hold your hand parallel to floor")
    okmessage ("Step 2", "Start counting down in 5s")
    app = ExampleApp()
    win.focus_set()

def cali_next():
    win.destroy()
    okmessage ("Step 4", "Prepare to hold your hand straight to the floor")
    okmessage ("Step 5", "Start counting down in 5s")
    app = ExampleApp()
    global win2
    win2 = Toplevel()
    win2.title('Step 6')
    message = "Please select the following options"
    Label(win2, text=message).pack()
    Button(win2, text='Redo', command=cali_redo_2).pack()
    Button(win2, text='Done', command=cali_done).pack()
    Button(win2, text='Cancel', command=cali_cancel_2).pack()

def cali_cancel_1():
    okmessage('Cancelled', 'Calibration has been cancelled')
    win.destroy()

def cali_redo_2():
    okmessage ("Step 4", "Prepare to hold your hand straight to the floor")
    okmessage ("Step 5", "Start counting down in 10s")
    app = ExampleApp()
    win2.focus_set()
    
def cali_cancel_2():
    okmessage('Cancelled', 'Calibration has been cancelled')
    win2.destroy()
    
def cali_done():
    okmessage('Done', 'Calibration successful')
    gyro_on=False
    #win2.destroy()


#***All Fingers Calibration *****
global thumb
def alf():
    okmessage ("Step 1", "Prepare to open your thumb as much as you can")
    okmessage ("Step 2", "Start counting down in 5s")
    app = ExampleApp()
    # t = Timer(5.0, popup1)
    # t.start()
    
def popup1():
    global win
    win = Toplevel()
    win.title('Step 3')
    print('yeah')
    message = "Please select the following options"
    Label(win, text=message).pack()    
    Button(win, text='Redo', command=alf_redo_1).pack()
    Button(win, text='Next', command=alf_next).pack()
    Button(win, text='Cancel', command=cali_cancel_1).pack()

def alf_redo_1():
    okmessage ("Step 1", "Prepare to open your thumb as much as you can")
    okmessage ("Step 2", "Start counting down in 10s")
    app = ExampleApp()
    win.focus_set()

def alf_next():
    win.destroy()
    okmessage ("Step 4", "Prepare to close your thumb as much as you can")
    okmessage ("Step 5", "Start counting down in 10s")
    app = ExampleApp()
    global win2
    win2 = Toplevel()
    win2.title('Step 6')
    message = "Please select the following options"
    Label(win2, text=message).pack()
    Button(win2, text='Redo', command=alf_redo_2).pack()
    Button(win2, text='Done', command=cali_done).pack()
    Button(win2, text='Cancel', command=cali_cancel_2).pack()

def alf_redo_2():
    okmessage ("Step 4", "Prepare to close your thumb as much as you can")
    okmessage ("Step 5", "Start counting down in 10s")
    app = ExampleApp()
    win2.focus_set()


#***Other Fingers Calibration *****

def infi():
    x="Prepare to point your other fingers horizontal to floor"
    okmessage ("Step 1", x)
    okmessage ("Step 2", "Start counting down in 10s")
    app = ExampleApp()
    global win
    win = Toplevel()
    win.title('Step 3')
    message = "Please select the following options"
    Label(win, text=message).pack()    
    Button(win, text='Redo', command=lambda:infi_redo_1(x)).pack()
    Button(win, text='Next', command=lambda:infi_next(fin)).pack()
    Button(win, text='Cancel', command=cali_cancel_1).pack()

def infi_redo_1(x):
    okmessage ("Step 1", x)
    okmessage ("Step 2", "Start counting down in 10s")
    app = ExampleApp()
    win.focus_set()

def infi_next(fin):
    win.destroy()
    y="Prepare to close your other fingers as much as you can"
    okmessage ("Step 4", y)
    okmessage ("Step 5", "Start counting down in 10s")
    app = ExampleApp()
    global win2
    win2 = Toplevel()
    win2.title('Step 6')
    message = "Please select the following options"
    Label(win2, text=message).pack()
    Button(win2, text='Redo', command=lambda:infi_redo_2(y)).pack()
    Button(win2, text='Done', command=cali_done).pack()
    Button(win2, text='Cancel', command=cali_cancel_2).pack()

def infi_redo_2(y):
    okmessage ("Step 4", y)
    okmessage ("Step 5", "Start counting down in 10s")
    win2.focus_set()


#**********Set Own Gestures Functions**********
    
#***Reset to default for gesture *****
    
def ResetD(ges):
    x="Reset to default for "+ges
    y="Do you want to enable gyroscope for "+ges
    ans = yes_question("Verify",x)
    if (ans==True):
        ans2 = yes_question("Gyro",y)
        if (ans2==True):
            ResetD2(ges,'T')
        else:
            ResetD2(ges,'F')
        okmessage ("Success","Function "+ges+"has been reset to default")

    else:
        okmessage('Cancelled', 'Reset for '+ges+' has been cancelled')

#Write function to go back to default stage, rewriting user modified to default
def ResetD2(ges, gyro):
    switcher={
        "moving hand": lambda:doNothing2(gyro),
        "left click": lambda:doNothing2(gyro),
        "double click": lambda:doNothing2(gyro),
        "right click": lambda:doNothing2(gyro),
        "scroll": lambda:doNothing2(gyro),
        "default stage": lambda:doNothing2(gyro),
        "quit": lambda:doNothing2(gyro),
        "reset all to default": lambda:doNothing2(gyro),
    }
    func=switcher.get(ges, lambda:'Invalid')
    return func()


#***Modify gesture *****

def mod(ges):
    y="Do you want to enable gyroscope for "+ges
    ans = yes_question("Gyro",y)
    x="Prepare to make your own "+ges+" function"
    okmessage ("Step 1", x)
    okmessage ("Step 2", "Start counting down in 5s")
    app = ExampleApp()
    global win
    win = Toplevel()
    win.title('Step 3')
    message = "Please select the following options"
    Label(win, text=message).pack()    
    Button(win, text='Redo', command=lambda:mod_redo(ges,win)).pack()
    Button(win, text='Next', command=lambda:mod_next_1(ges)).pack()
    Button(win, text='Cancel', command=mod_cancel_1).pack()

def mod_next_1(ges):
    win.destroy()
    y="Prepare to repeat your "+ges+ " function"
    okmessage ("Step 4", y)
    okmessage ("Step 5", "Start counting down in 5s")
    app = ExampleApp()
    global win2
    win2 = Toplevel()
    win2.title('Step 6')
    message = "Please select the following options"
    Label(win2, text=message).pack()
    Button(win2, text='Redo', command=lambda:mod_redo(ges,win2)).pack()
    Button(win2, text='Next', command=lambda:mod_next_2(ges)).pack()
    Button(win2, text='Cancel', command=mod_cancel_2).pack()

def mod_next_2(ges):
    win2.destroy()
    y="Prepare to repeat your "+ges+ " function"
    okmessage ("Step 7", y)
    okmessage ("Step 8", "Start counting down in 5s")
    app = ExampleApp()
    global win3
    win3 = Toplevel()
    win3.title('Step 9')
    message = "Please select the following options"
    Label(win3, text=message).pack()
    Button(win3, text='Redo', command=lambda:mod_redo(ges,win3)).pack()
    Button(win3, text='Done', command=mod_done).pack()
    Button(win3, text='Cancel', command=mod_cancel_3).pack()

def mod_redo(ges,win_a):
    win_a.destroy()
    x="Prepare to make your own "+ges+" function"
    okmessage ("Step 1", x)
    okmessage ("Step 2", "Start counting down in 5s")
    app = ExampleApp()
    global win
    win = Toplevel()
    win.title('Step 3')
    message = "Please select the following options"
    Label(win, text=message).pack()    
    Button(win, text='Redo', command=lambda:mod_redo(ges,win)).pack()
    Button(win, text='Next', command=lambda:mod_next_1(ges)).pack()
    Button(win, text='Cancel', command=mod_cancel_1).pack()
    
def mod_cancel_1():
    okmessage('Cancelled', 'Gesture change has been cancelled')
    win.destroy()
  
def mod_cancel_2():
    okmessage('Cancelled', 'Gesture change has been cancelled')
    win2.destroy()

def mod_cancel_3():
    okmessage('Cancelled', 'Gesture change has been cancelled')
    win3.destroy()
    
def mod_done():
    okmessage('Done', 'Gesture change successful')
    win3.destroy()


#**********Quit Function**********
def quit(root):
    ans = yes_question("Verify", "Do you really want to exit WHID")
    if (ans==True):
        okmessage ("Thank you", "See you next time")
        #global root
        root.quit()
    else:
        okmessage('Cancelled', 'Quit has been cancelled')

    
#**********Help Function**********
def about():
    global about
    about = Toplevel()
    about.title('WHID')
    S=Scrollbar(about)
    T = Text(about, height=4, width=50)
    S.pack(side=RIGHT, fill=Y)
    T.pack(side=LEFT, fill=Y)
    S.config(command=T.yview)
    T.config(yscrollcommand=S.set)
    guide = """The W.H.I.D. is a hand wearable human interface device that allows users to interact with Windows PC.
Worn over the hand, gestures send signals that inact behaviors such as controlling the virtual mouse.
Allows users to utilize computer systems from a distance given potential scenarios where this may be necessary.
This is a general purpose device for any users.
Any and all users should see some gain from the utilization of a more customizable
and personal user interface device by allowing users to remotely interact with the system with their preference of gestures.
Default gestures for W.H.I.D. is gyroscope enabled and gestures as followed:

User Side                                 Computer Side
Moving hand                               Moves the cursor
Right index finger drops twice            Double click
Index finger drop                         Right Click
Middle finger                             Left Click
Move hand                                 Scroll
Index finger stayed holded                Mouse Drag
Hardware/turn off                         Exit mode
Flat                                      Default Stage (cursor in middle)
"""
    T.insert(END, guide)

   
global root


def generate(root):

    # ***** Main Menu *****
    # root = Tk()
    menu = Menu(root)
    root.config(menu=menu)
    root.title('WHID')
    root.geometry("500x500")

    # *** Calibration Menu ***

    cali_Menu = Menu(menu)
    menu.add_cascade(label="Calibrate", menu=cali_Menu)

    cali_Menu.add_command(label="Gyroscope", command=gyro)

    fin_Menu = Menu(cali_Menu)
    cali_Menu.add_cascade(label="Finger", menu=fin_Menu)
    fin_Menu.add_command(label="Thumb", command=alf)
    trail = "Thumb"
    ind_Menu = Menu(fin_Menu)
    fin_Menu.add_cascade(label="All fingers beside thumb", command=infi)
    
    #add ending separators
    cali_Menu.add_separator()
    fin_Menu.add_separator()
    ind_Menu.add_separator()

    # *** Gesture Change Menu ***

    edit_Menu = Menu(menu)
    menu.add_cascade(label="Gesture", menu=edit_Menu)

    mov_Menu = Menu(edit_Menu)
    edit_Menu.add_cascade(label="Moving Hand", menu=mov_Menu)
    mov_Menu.add_command(label="Reset to Default", command=lambda:ResetD("moving hand"))
    mov_Menu.add_command(label="Set own moving hand", command=lambda:mod("moving hand"))

    left_Menu = Menu(edit_Menu)
    edit_Menu.add_cascade(label="Left Click", menu=left_Menu)
    left_Menu.add_command(label="Reset to Default", command=lambda:ResetD("left click"))
    left_Menu.add_command(label="Set own left click", command=lambda:mod("left click"))

    dou_Menu = Menu(edit_Menu)
    edit_Menu.add_cascade(label="Double Click", menu=dou_Menu)
    dou_Menu.add_command(label="Reset to Default", command=lambda:ResetD("double click"))
    dou_Menu.add_command(label="Set own double click", command=lambda:mod("double click"))

    right_Menu = Menu(edit_Menu)
    edit_Menu.add_cascade(label="Right Click", menu=right_Menu)
    right_Menu.add_command(label="Reset to Default", command=lambda:ResetD("right click"))
    right_Menu.add_command(label="Set own right click", command=lambda:mod("right click"))

    scroll_Menu = Menu(edit_Menu)
    edit_Menu.add_cascade(label="Scroll", menu=scroll_Menu)
    scroll_Menu.add_command(label="Reset to Default", command=lambda:ResetD("scroll"))
    scroll_Menu.add_command(label="Set own scroll", command=lambda:mod("scroll"))

    def_Menu = Menu(edit_Menu)
    edit_Menu.add_cascade(label="Default stage", menu=def_Menu)
    def_Menu.add_command(label="Reset to Default", command=lambda:ResetD("default stage"))
    def_Menu.add_command(label="Set own double click", command=lambda:mod("default stage"))

    quit_Menu = Menu(edit_Menu)
    edit_Menu.add_cascade(label="Quit", menu=quit_Menu)
    quit_Menu.add_command(label="Reset to Default", command=lambda:ResetD("quit"))
    quit_Menu.add_command(label="Set own quit", command=lambda:mod("quit"))

    edit_Menu.add_command(label="Reset all to default", command=lambda:ResetD("reset all to default"))

    #add ending separators
    edit_Menu.add_separator()
    mov_Menu.add_separator()
    left_Menu.add_separator()
    dou_Menu.add_separator()
    right_Menu.add_separator()
    scroll_Menu.add_separator()
    def_Menu.add_separator()
    quit_Menu.add_separator()

    # *** Quit Menu ***
    menu.add_command(label="Quit", command=lambda:quit(root))

    # *** Reconnect Menu ***
    menu.add_command(label="Reconnect", command=doNothing)

    # *** Help Menu ***
    help_Menu = Menu(menu)
    menu.add_cascade(label="Help", menu=help_Menu)
    help_Menu.add_command(label="About WHID", command=about)

    # ***** The Toolbar *****

    toolbar = Frame(root, bg="blue")

#**********Counter Functions**********

class ExampleApp(tk.Tk):
    def __init__(self):
        tk.Tk.__init__(self)
        self.label = tk.Label(self, text="", width=10)
        self.label.pack()
        self.remaining = 0
        self.countdown(5)
        
    global time_up
    def countdown(self, remaining = None):
        count=1

        time_up=-1
        if remaining is not None:
            self.remaining = remaining

        if self.remaining <= 0:
            time_up=0
            self.label.configure(text="time's up!")
            self.destroy()
        else:
            self.label.configure(text="%d" % self.remaining)
            self.remaining = self.remaining - 1
            self.after(1000, self.countdown)
        
    def getter():
        return time_up
    

if __name__ == "__main__":
    root=Tk()
    generate(root)
    root.mainloop()
