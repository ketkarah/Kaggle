#!/usr/bin/env python
import sys
import csv
from collections import Counter
import pandas as pd

wordcount={}
stream=[]

def word(cl):
    wlshin=wordshingles(cl)
    df=pd.read_csv("/home/training/Desktop/Project/Words.csv",encoding='latin')
    polist=df["Positive_words"].values.tolist()
    neglist=df["Negative_words"].values.tolist()
    poshingles=pos_neg_shingles(polist)
    negshingles=pos_neg_shingles(neglist)
    return wlshin,poshingles,negshingles,polist,neglist

def wordshingles(wordlist):
    j=0
    wordshingles={}
    while j<len(wordlist):
       temp=wordlist[j].split()
       i=0
       while i<len(temp):      
             tem={temp[i][l:l+9] for l in range(0,len(temp[i])-1)}
             tem=list(tem)
             k=0
             while k<len(tem):
                if tem[k] not in wordshingles:
                   wordshingles[tem[k]]=1
                else:
                   wordshingles[tem[k]]+=1
                k=k+1
             i=i+1
       j=j+1
    return wordshingles

def pos_neg_shingles(list1):
    j=0
    shingles={}
    while j<len(list1):
       temp=list1[j]
       tem={temp[i:i+9] for i in range(0,len(temp)-1)}
       tem=list(tem)
       k=0
       while k<len(tem):
          if tem[k] not in shingles:
              shingles[tem[k]]=1
          else:
              shingles[tem[k]]+=1
          k=k+1
       j=j+1
    return shingles

def misc_names(wc,file):
    wlist=open(file,encoding='latin')
    header=csv.reader(wlist)
    next(header)
    colist=[]
    for line in header:
        colist.append(line[0])
    countries={}
    i=0
    while i<len(colist):
       countries[colist[i]]=1
       i=i+1
    final_dict={}
    for key,value in wc.items():
       if key in countries.keys():        
          final_dict[key]=value
    return final_dict

def counter(w,ps,ns):
    count1=0
    count2=0
    for key in ps.keys():
        if key in w.keys():
           count1+=1

    for key in ns.keys():
        if key in w.keys():
           count2+=1
    psp=count1/len(w)
    nsp=count2/len(w)
    pj1=Jaccard_similarity(ps,w,count1)
    nj1=Jaccard_similarity(ns,w,count2)
    return psp,nsp,pj1,nj1

def sort_dict(my_dict):
    c=Counter(my_dict)
    return c.most_common(25)

def Jaccard_similarity(shing,w,pj):
    return pj/(len(w)+len(ps)-pj)

def word_similarity(wordcount,pl,nl):
    count1=0
    count2=0
    for word in wordcount.keys():
        if word in pl: 
           count1+=1
        elif word in nl:
           count2+=1
    return count1/len(wordcount),count2/len(wordcount)
         
for line in sys.stdin:
    line=line.strip()
    stream.append(line)
    if line not in wordcount:
        wordcount[line]=1
    else:
        wordcount[line]+=1
w,ps,ns,pl,nl=word(stream)
plpc,nlpc=word_similarity(wordcount,pl,nl)
cntry=misc_names(wordcount,"/home/training/Desktop/Project/Countries.csv")
psp,nsp,pj,nj=counter(w,ps,ns)
sort_wc=sort_dict(wordcount)
sort_cntry=sort_dict(cntry)
print("25 most common words-------------")
print(sort_wc)
print("25 most common miscellaneous names-------------")
print(sort_cntry)
print("%Positive Words without shingling:",plpc*100)
print("%Negative Words without shingling:",nlpc*100)
print("%Positive Words with shingling:",psp*100)
print("%Negative Words with shingling:",nsp*100)
print("%Jaccard similarity index of Positive words:",pj*100)
print("%Jaccard similarity index of Negative words:",nj*100)
