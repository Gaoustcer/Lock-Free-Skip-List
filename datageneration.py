import random
import string
from random import randint
from random import random as coin
import os
'''
do NUM_OPERATION operations
OPTIONS indicate type of operation
I insert F find
1. 0.5 probability Insert
2. 0.5 probability F
    1. 0.5 find a existing value
    2. 0.5 fail to find
'''

MAXVALUE = 1024 * 32
INITINSERT = 256

NUM_OPERATION = 1024

OPTIONS = ["I","F"]
DATAPATH = "./data"
def randstr(len = 10):
    rstr = ''.join(random.sample(string.ascii_letters + string.digits,len))
    return rstr

def generatetestdata():
    savedict = dict()
    operationdict = []
    result = []
    for _ in range(INITINSERT):
        s = randstr()
        value = randint(0,MAXVALUE)
        operationdict.append(["I",s,value])
        operationdict.append(["F",s])
        result.append(value)
        savedict[s] = value
    for _ in range(NUM_OPERATION):
        operation = random.choice(OPTIONS)
        if operation == "I":
            s = randstr()
            value = randint(0,MAXVALUE)
            savedict[s] = value
            operationdict.append(["I",s,value])
        elif operation == "F":
            if coin() < 0.5:
                s = random.choice(list(savedict.keys()))
                value = savedict[s]
                # .append(s)
                operationdict.append(["F",s])
                result.append(value)
            else:
                s = randstr()
                result.append(-1)
                operationdict.append(["F",s])
    return operationdict,result


if __name__ == "__main__":
    if os.path.exists(DATAPATH) == False:
        os.mkdir(DATAPATH)
    oper,res = generatetestdata()
    print("operation",oper)
    print("result",res)
    with open(f"{DATAPATH}/operations.txt","w") as fp:
        for operation in oper:
            s = ''
            for x in operation:
                s += str(x) + " "
            # print(s)
            s += "\n"
            # s = s.encode()
            fp.write(s)
    with open(f'{DATAPATH}/result.txt','w') as fp:
        # fp.write(str(res))
        for operation in res:
            operation = str(operation) + "\n"
            fp.write(operation)
    