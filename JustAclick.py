import pyautogui
import API_Imple

#this is a click
ma = 55
mi = 12
cur = 33
temp = maxN - curr
flag_clk = 0
cnt_clk = 0
mclk = False
maxCnt = 10 #initial value

def percent(mi,ma,cur):
    flag = True
    temp = ma - cur
    if ((temp/(ma-mi))*100) >= 50:
        flag = True
    else:
        flag = False
    return flag

##+++++++++++++++++++++++++++++++++++++++++##

mclk = percent(mi,ma,cur)
if(flag_clk == 0):
    if (mclk == True):
        API_Imple.ldown()
        flag_clk = 1
        cnt_clk = 0
if(flag_clk ==1 and cnt_clk == 0):
    if(mclk == False):
        API_Imple.lup()
        cnt_clk += 1
    elif(mclk == True and cnt_clk < maxCnt):
        API_Imple.ldown()
        flag_clk = 0
    elif (cnt_clk >= maxCnt):
        flag_clk = 0
