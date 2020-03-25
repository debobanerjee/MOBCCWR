# -*- coding: utf-8 -*-
"""
Created on Wed Mar 25 15:06:05 2020

@author: Shubham
"""

import PIL
from PIL import ImageTk
import requests
from io import BytesIO
from tkinter import *  
from tkinter import messagebox
from urllib.request import urlopen, Request
import numpy as np
import cv2
import csv
import subprocess

def url_to_image(url, readFlag=cv2.IMREAD_COLOR):
    # download the image, convert it to a NumPy array, and then read
    # it into OpenCV format
    req = Request(url=url, headers = headers)
    resp = urlopen(req)
    image = np.asarray(bytearray(resp.read()), dtype="uint8")
    image = cv2.imdecode(image, readFlag)

    # return the image
    return image

def greedyAlgo(budget, alpha, beta):
    p = subprocess.Popen(["b.exe"], stdout = subprocess.PIPE, stdin = subprocess.PIPE)
    file = open("amazon_input.txt")
    text = file.read()
    text += " " + str(alpha) + " " + str(beta) + " " + str(budget)
    output = p.communicate(input = text.encode())[0].decode()
    OUTFITS = []
    
    print("VCFREC3 output: ")
    print(output)
    
    s = ""
    outfit = []
    for row in output:
        if row == " " or row == '\n':
            outfit.append(int(s))
            s = ""
        if row == '\n':
            new = outfit
            OUTFITS.append(new)
            outfit = []
            continue
        
        if row == " ":
            continue;
        s += row;
    
    return OUTFITS
    

fixUrl = "https://cse.iitkgp.ac.in/~debopriyob/BOXREC/Amazon/images/"
headers = {'User-Agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.64 Safari/537.11',
       'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
       'Accept-Charset': 'ISO-8859-1,utf-8;q=0.7,*;q=0.3',
       'Accept-Encoding': 'none',
       'Accept-Language': 'en-US,en;q=0.8',
       'Connection': 'keep-alive'}

def getUrls(idx, x):
    urls = []
    for i in idx:
        urls.append(fixUrl + items[i[x] - 1])
    return urls
    
def select():
    if len(budgetTxt.get()) == 0:
        messagebox.showinfo('Error', 'Enter budget')
        return
    BUDGET = int(budgetTxt.get())
    if BUDGET == 0:
        messagebox.showinfo('Error', 'Enter non-zero budget')
        return
    
    ALPHA = int(alpha.get())
    BETA = int(beta.get())
    
    if ALPHA == 0 and BETA == 0:
        messagebox.showinfo('Error', 'Both parameters are zero.')
        return
    
    outfitIdx = greedyAlgo(BUDGET, ALPHA, BETA)
    
    print("Oufit idx: ")
    print(outfitIdx)
    
    topUrls = getUrls(outfitIdx, 0)
    bottomUrls = getUrls(outfitIdx, 1)
    footUrls = getUrls(outfitIdx, 2)
    
    display_image(topUrls, bottomUrls, footUrls)
    
def add():
    file = open("result.txt", "a")
    print(str(take.get()))
    file.write(str(take.get()) + '\n');
    
def opinion():
    opLabel = Label(window, width = 40, text = "Recommended set satisfability (from 0 to 10): ", anchor = 'w')
    opScale = Scale(window, variable = take, from_ = 0, to = 10, orient = HORIZONTAL)
    opLabel.grid(column = 0, row = 6)
    opScale.grid(column = 1, row = 6)
    
    btn = Button(window, width = 15, text='Submit', command=add, anchor=CENTER)
    btn.grid(column = 0, row = 8)
    
    

def display_image(urltop, urlbottom, urlfoot):
    isFirst = TRUE;
    for url in urltop:
        print("downloading %s" % (url))
        image = url_to_image(url)
        if isFirst == TRUE:
            top = image
            isFirst = FALSE
            continue
        top = np.concatenate((top, image), axis = 1)
    cv2.imshow("Top wear", top)
    
    isFirst = TRUE
    for url in urlbottom:
        print("downloading %s" % (url))
        image = url_to_image(url)
        if isFirst == TRUE:
            bottom = image
            isFirst = FALSE
            continue
        bottom = np.concatenate((bottom, image), axis = 1)
    cv2.imshow("Bottom wear", bottom)
    
    isFirst = TRUE
    for url in urlfoot:
        print("downloading %s" % (url))
        image = url_to_image(url)
        if isFirst == TRUE:
            foot = image
            isFirst = FALSE
            continue
        foot = np.concatenate((foot, image), axis = 1)
    cv2.imshow("Foot wear", foot)
    
    outfit = top
    outfit = np.concatenate((outfit, bottom), axis = 0)
    outfit = np.concatenate((outfit, foot), axis = 0)
    cv2.imshow("Outfits", outfit)
    
    opinion()
    
    
items = []

file = open("reverse_hashing.txt")
text = file.read()

s = ""

for row in text:
    if row == '\n':
        items.append(s)
        s = ""
        continue
    s += row
        
window = Tk()  
window.geometry('650x300')
window.title('VCFREC Demo')
  
alpha = DoubleVar()
beta = DoubleVar()
take = DoubleVar()

budgetLabel = Label(window, width = 22, text="Budget (in Rs > 1000): ", anchor = 'w')
budgetTxt = Entry(window)
budgetLabel.grid(column = 0, row = 0)
budgetTxt.grid(column = 1, row = 0)

alphaLabel = Label(window, width = 40, text = "Alpha (Weight for compatibility in %): ", anchor = 'w')
alphaScale = Scale(window, variable = alpha, from_ = 0, to = 100, orient = HORIZONTAL)
alphaLabel.grid(column = 0, row = 1)
alphaScale.grid(column = 1, row = 1)

row = Frame(window)
betaLabel = Label(window, width = 40, text = "Beta (Weight for versatility in %): ", anchor = 'w')
betaScale = Scale(window, variable = beta, from_ = 0, to = 100, orient = HORIZONTAL)
betaLabel.grid(column = 0, row = 2)
betaScale.grid(column = 1, row = 2)

btn = Button(window, width = 15, text='Go!', command=select, anchor=CENTER)
btn.grid(column = 0, row = 4)

lbl = Label(window)
lbl.grid(column = 0, row = 5)

window.mainloop()  