import random
i=0
array=(1,2,3)
count=[0,0,0]
while i < 1000:
    #print i,":"
    choice=random.choice([1,2,3])
    i=i+1
    if choice==1:
        count[0]=count[0]+1
    elif choice == 2:
        count[1]=count[1]+1
    else:
        count[2]=count[2]+1

for i in count:
    print i
