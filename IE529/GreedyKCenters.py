import csv
import numpy as np
import random as rd
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import matplotlib.cm as cmx
import timeit
k=2
def dist(pv,a):
    kcenter=a
    i=0
    j=0
    l=0
    dist=[]
    findist=[]
    while l<k-1:
       i=0
       while i<len(pv):
            j=0
            while j<len(kcenter):
                dist.append(((pv[i][0]-kcenter[j][0])**2 + (pv[i][1]-kcenter[j][1])**2)**0.5)
                j=j+1
            i=i+1
       w=np.argmax(dist)
       findist.append(dist[w])
       w=divmod(w,k-1)
       if w[1]==0:
           quo=w[0]
       else:
           quo=w[0]+1
       dist=[]
       kcenter=np.vstack((kcenter,pv[quo]))
       l=l+1
    return findist, kcenter
    
def distance(pv,a):
    i=0
    j=0
    distemp=[]
    index=[]
    dist=[]
    while i<len(pv):
        while j<len(a):
            distemp.append(((pv[i][0]-a[j][0])**2 + (pv[i][1]-a[j][1])**2)**0.5)
            dist.append(((pv[i][0]-a[j][0])**2 + (pv[i][1]-a[j][1])**2)**0.5)
            j=j+1
        w=np.argmin(distemp)
        index.append(w)
        distemp=[]
        i=i+1
        j=0
    dist=np.array(dist)
    index=np.array(index)
    return dist,index

def assign(pv,index):
    i=0
    clubtemp=[]
    club=[]
    while i<k:
        w=np.where(i==index)
        j=0
        while j<len(pv):
            if j in w[0]:
                clubtemp.append(pv[j])
                j=j+1
                continue
            else:
                j=j+1
                continue
        club.append(clubtemp)
        clubtemp=[]
        i=i+1
    return club
start=timeit.default_timer()
with open("C:/Users/Akshay/Desktop/Sem 3/Becky/IE529CA2/Clustering.csv")as csvfile:
  b=csv.reader(csvfile)
  pcom=[]
  dcom=[]
  for row in b:
      pcom.append(row[0])
      dcom.append(row[1])
      
i=0
av=[]
dv=[]
pv=[]
pvindex=[]
while i<len(pcom):
     av.append(float(pcom[i]))
     dv.append(float(dcom[i]))
     pv.append([av[i],dv[i]])
     i=i+1
a=rd.sample(pv,1)
a=np.array(a)
pv=np.array(pv)
findist,kcenter=dist(pv,a)
d,index=distance(pv,kcenter)
club=assign(pv,index)
pv1temp=[]
pv2temp=[]
pv1=[]
pv2=[]

l=0
m=0
while l<k:
    m=0
    while m<len(club[l]):
        pv1temp.append(club[l][m][0])
        pv2temp.append(club[l][m][1])
        m=m+1
    pv1.append(pv1temp)
    pv2.append(pv2temp)
    pv1temp=[]
    pv2temp=[]
    l=l+1

hot = plt.get_cmap('hot')
cNorm  = colors.Normalize(vmin=0, vmax=k)
scalarMap = cmx.ScalarMappable(norm=cNorm, cmap=hot)
xs=0
while xs < k:
    plt.scatter(pv1[xs], pv2[xs], s=15,color=scalarMap.to_rgba(xs))
    plt.scatter(kcenter[xs][0],kcenter[xs][1])
    xs=xs+1
stop = timeit.default_timer()
print(stop-start) 


