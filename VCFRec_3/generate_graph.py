# -*- coding: utf-8 -*-
"""
Created on Wed Mar 25 12:06:39 2020

@author: Shubham
"""

import csv

outfits = []
odd_row = True
is_first_row = True

with open('outfit_data.csv', 'rt') as f:
    data = csv.reader(f)
    for row in data:
        if odd_row == True:
            odd_row = False
            if is_first_row == True:
                is_first_row = False
                continue
            outfits.append(row)
            continue
        odd_row = True

file = open("input_graph.txt", "w")

file.write(str(len(outfits)) + '\n');

for row in outfits:
    file.write(row[0] + ' ')
    file.write(row[1] + ' ')
    file.write(row[2] + ' ')
    file.write(row[3] + ' ')
    file.write(row[4] + ' ')
    file.write(row[5] + ' ')
    occasions = row[6].split(',')
    file.write(str(len(occasions)) + ' ')
    for u in occasions:
        file.write(u + ' ')
    file.write('\n')
    
file.close()
