from itertools import combinations
from collections import Counter
import sys

def create(dataset):
    "Create a list of candidate item sets of size one."
    c1 = []
    for transaction in dataset:
        for item in transaction:
            if not [item] in c1:
                c1.append([item])
    c1.sort()
    #frozenset because it will be a ket of a dictionary.
    return map(frozenset, c1)

def candidates(D,c2,min_support):
    "Returns all candidates that meets a minimum support level"
    sscnt = {}
    for tid in D:
        for can in c2:
            if can.issubset(tid):
                sscnt.setdefault(can, 0)
                sscnt[can] += 1
                

    num_items = float(len(D))
    retlist = []
    support_data = {}
    for key in sscnt:
        support = sscnt[key]
        if support >= min_support:
            retlist.insert(0, key)
        support_data[key] = support
    return retlist, support_data

def ClosedPattern(itemss,New_L):
    closedItemset = {}
    SameSup = [tuple(itemss.items()[0][0])]
    sup = itemss.items()[0][1]
    for i in range(1,len(itemss)):
        if itemss.items()[i][1]==sup:
            SameSup.append(tuple(itemss.items()[i][0]))      
        else:
            for j in range(0,len(SameSup)):
                item1Fields = SameSup[j]
                isClosed = True
                for k in range( 0, len(SameSup) ):
                    if k != j:
                        item2Fields = SameSup[k]
                        match = 0
                        for item1 in item1Fields:
                            for item2 in item2Fields:
                                if item1 == item2:
                                    match += 1
                                    break
                        if match == len(item1Fields):
                            isClosed = False
                            break
                if isClosed:
                    closedItemset[SameSup[j]] = sup
            SameSup = [tuple(itemss.items()[i][0])]  
            sup = itemss.items()[i][1]
    for f in range( 0, len(SameSup) ):
        item1Fields = SameSup[f]
        isClosed = True
        for k in range( 0, len(SameSup) ):
            if k != f:
                item2Fields = SameSup[k]
                match = 0
                for item1 in item1Fields:
                    for item2 in item2Fields:
                        if item1 == item2:
                            match += 1
                            break
                if match == len(item1Fields):
                    isClosed = False
                    break
        if isClosed:
            closedItemset[SameSup[f]] = sup
            
    cIt={}
    
    for i in range(0,len(closedItemset)):
        for j in range(0,len(closedItemset)):
            if j !=i:
                if set(closedItemset.items()[i][0]).issubset(set(closedItemset.items()[j][0])) and closedItemset.items()[i][1]<=closedItemset.items()[j][1]:
                    cIt[closedItemset.items()[i][0]]=closedItemset.items()[i][1]

    for i,j in closedItemset.items():
        if i in cIt:
            del(closedItemset[i])
    return closedItemset
    
def MaxPattern(ClPat):
    MP={}
    for i in range(0,len(ClPat)):
        for j in range(0,len(ClPat)):
            if j !=i:
                if set(ClPat.items()[i][0]).issubset(set(ClPat.items()[j][0])):
                    MP[ClPat.items()[i][0]]=ClPat.items()[i][1]
    
    MaxItemset=ClPat
    for i,j in MaxItemset.items():
        if i in MP:
            del(MaxItemset[i])
    return MaxItemset
    
                       
def apriori(freq_sets, k):
    "Generate the joint transactions from candidate sets"
    retList = []
    lenLk = len(freq_sets)
    for i in range(0,lenLk):
        for j in range(i + 1, lenLk):
            L1 = list(freq_sets[i])
            L2 = list(freq_sets[j])
            L1.sort()
            L2.sort()
            L1 = L1[:k - 2]
            L2 = L2[:k - 2]
            if L1 == L2:
                retList.append(freq_sets[i] | freq_sets[j])
    return retList
arr=[]
for line in sys.stdin:
    arr.append(list(map(str, line.split())))
    min_sup=arr[0]
    min_support=int(min_sup[0])
    arr1=arr[1:]
C1 = create(arr1)
D = map(set, arr1)
L1,support_data = candidates(D,C1,min_support)
L=[L1]
k = 2
while (len(L[k - 2]) > 0):
    Ck = apriori(L[k - 2], k)
    Lk, supK = candidates(D,Ck, min_support)
    support_data.update(supK)
    L.append(Lk)
    k += 1

for k, v in support_data.items():
    if v < min_support:
        del(support_data[k])
        
itemss={}
vn=[]
for k, v in support_data.items():
    k=sorted(k)
    vn.append(k)
    itemss[tuple(k)]=v 
    
NL=map(frozenset,vn)
ClPat=ClosedPattern(itemss,NL)

sortedList = sorted(itemss.items(), key=lambda k: (-k[1], k[0]))
sortedList1 = sorted(ClPat.items(), key=lambda l: (-l[1], l[0]))
for k in sortedList:
    ka=k[0]    
    print k[1],"["+' '.join(ka).replace('"','')+"]"
print("")
for l in sortedList1:
    la=l[0]    
    print l[1],"["+' '.join(la).replace('"','')+"]"
print("")
MPat=MaxPattern(ClPat)
sortedList2 = sorted(MPat.items(), key=lambda m: (-m[1], m[0]))
for m in sortedList2:
    ma=m[0]    
    print m[1],"["+' '.join(ma).replace('"','')+"]"
