import math
import sys
def ent(data):
    val_freq     = {}
    data_entropy = 0.0
    for record in data:
        if (val_freq.has_key(record[-1])):
            val_freq[record[-1]] += 1.0
        else:
            val_freq[record[-1]]  = 1.0

    for freq in val_freq.values():
        data_entropy += (-freq/len(data)) * math.log(freq/len(data), 2) 
        
    return data_entropy

def GainandGainRatio(data,attr):
    val_freq       = {}
    subset_entropy = 0.0

    for record in data:
        if (val_freq.has_key(record[attr])):
            val_freq[record[attr]] += 1.0
        else:
            val_freq[record[attr]]  = 1.0   
    for val in val_freq.keys():       
        val_prob        = val_freq[val] / sum(val_freq.values())
        data_subset     = [record for record in data if record[attr] == val]
        subset_entropy += val_prob * ent(data_subset)    
    sp_entropy=0.0   
    for sp in val_freq.values():
        sent=sp/sum(val_freq.values())
        sp_entropy+=(-sent)*math.log(sent, 2)
    if sp_entropy==0.0:
        return (ent(data) - subset_entropy),"inf"
    else:
        return (ent(data) - subset_entropy),(ent(data) - subset_entropy)/sp_entropy

arr=[]
for line in sys.stdin:
    arr.append(list(map(str,line.split(','))))
    
arr1=arr[2:]
arr2=[]
for i in arr1:
    list2 = [el.replace('\n', '') for el in i]
    arr2.append(list2)    
de=ent(arr2)
Infgain=[]
GRatio=[]
Ilist=arr[1]
Ilist1=[el.replace("\n",'') for el in Ilist]
k=0
while k < len(Ilist1)-1: 
    a,b=GainandGainRatio(arr2,k)
    Infgain.append(a)
    GRatio.append(b)
    k=k+1

Info={}
GRat={}
i=0
while i<len(Infgain):
    Info[Ilist1[i]]=Infgain[i]
    GRat[Ilist1[i]]=GRatio[i]
    i=i+1 

print(max(Info, key=Info.get))
print(max(GRat, key=GRat.get))

###Input form:

15
age,income,student,creditrating,buyscomputer?
l30,high,no,fair,no
l30,high,no,excellent,no
31to40,high,no,fair,yes
g40,medium,no,fair,yes
g40,low,yes,fair,yes
g40,low,yes,excellent,no
31to40,low,yes,excellent,yes
l30,medium,no,fair,no
l30,low,yes,fair,yes
g40,medium,yes,fair,yes
l30,medium,yes,excellent,yes
31to40,medium,no,excellent,yes
31to40,high,yes,fair,yes
g40,medium,no,excellent,no

