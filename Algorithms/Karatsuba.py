def Karatsuba(a,b):
    if len(str(a))==1 or len(str(b))==1:
        return a*b
        
    else:
       n=len(str(a))
       nb2=int(n/2)
       
       l_a=a//(pow(10,nb2))
       r_a=a%(pow(10,nb2))
       l_b=b//(pow(10,nb2))
       r_b=b%(pow(10,nb2))

       if len(str(l_a))!=nb2:
           l_a = str(0)+str(l_a)
           l_a=int(l_a)
       
       if len(str(r_a))!=nb2:
           r_a = str(0)+str(r_a)
           r_a=int(r_a)
       
       if len(str(l_b))!=nb2:
           l_b = str(0)+str(l_b)
           l_b=int(l_b)
       
       if len(str(r_b))!=nb2:
           r_b = str(0)+str(r_b)
           r_b=int(r_b)
       
       ac=Karatsuba(l_a,l_b)
       bd=Karatsuba(r_a,r_b)
       ad=Karatsuba(l_a,r_b)
       bc=Karatsuba(r_a,l_b)
       fin=pow(10,n)*ac + pow(10, n/2)*(ad + bc) + bd
    return fin
    
a=3141592653589793238462643383279502884197169399375105820974944592
b=2718281828459045235360287471352662497757247093699959574966967627
fin=int(Karatsuba(a,b))
