#!/usr/bin/env python
import csv
import sys

cl=open("/home/training/Desktop/Project/Clean.csv",'r')
header=csv.reader(cl)
next(header)
word=[]
for line in header:
    word.append(line[4])

for line in word:
    words=line.split()
    for wd in words:
        print(wd)
