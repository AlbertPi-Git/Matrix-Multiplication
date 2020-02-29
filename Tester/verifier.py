import numpy as np

m1,m2,res=[],[],[]
cnt=0
lineCnt=0
with open('test_output.txt','r') as f:
    for line in f:
        lineCnt+=1
        if line=='\n':
            cnt+=1
            continue
        if cnt==0:
            m1.append([float(char) for char in line.split()])
        elif cnt==1:
            m2.append([float(char) for char in line.split()])
        else:
            res.append([float(char) for char in line.split()])

epsilon=1e-5
m1,m2,res=np.array(m1),np.array(m2),np.array(res)
equal=True
npRes=np.matmul(m1,m2)
for i in range(len(res)):
    for j in range(len(res[0])):
        if abs(res[i][j]-npRes[i][j])>epsilon:
            equal=False
            break

if equal:
    print("Test passed!")
else:
    print("Test failed!")
                