import random
import string
from random import randint
from random import random as coin
from random import choice
from tqdm import tqdm
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

NUM_OPERATION = 1024 * 128 # number of insert/delete

OPTIONS = ["I","F","D"]
DATAPATH = "./data"
def randstr(len = 10):
    rstr = ''.join(random.sample(string.ascii_letters + string.digits,len))
    return rstr

keyvaluemap = dict()
operation = []
def constructskiplist():
    for _ in tqdm(range(NUM_OPERATION)):
        # op = choice(["I","D"])
        if coin() < 0.8:
            op = "I"
            key = randstr()
            value = randint(0,MAXVALUE)
            keyvaluemap[key] = value
            operation.append([op,key,value])
        elif  len(list(keyvaluemap.keys())):
            op = "D"
            if coin() < 0.5:
                key = choice(list(keyvaluemap.keys()))
                del keyvaluemap[key]
            else:
                key = randstr()
            operation.append([op,key])
SEARCHTIME = NUM_OPERATION * 2
result = []
def search():
    for _ in tqdm(range(SEARCHTIME)):
        op = "F"
        if coin() < 0.5:
            key = choice(list(keyvaluemap.keys()))
            value = keyvaluemap[key]
            result.append([key,value])
        else:
            key = randstr()
            value = -1
            result.append([key,value])
        operation.append([op,key])



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
    from tqdm import tqdm
    for _ in tqdm(range(NUM_OPERATION)):
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
        elif operation == "D" and len(savedict.keys()):
            s = random.choice(list(savedict.keys()))
            del savedict[s]
            operationdict.append(["D",s])

    return operationdict,result


if __name__ == "__main__":
    if os.path.exists(DATAPATH) == False:
        os.mkdir(DATAPATH)
    # oper,res = generatetestdata()
    # print("operation",oper)
    # print("result",res)
    constructskiplist()
    search()
    with open(f"{DATAPATH}/savedict.pkl",'wb') as fp:
        import pickle 
        pickle.dump(keyvaluemap,fp)
    with open(f"{DATAPATH}/operations.txt","w") as fp:
        for op in operation:
            s = ''
            for x in op:
                s += str(x) + " "
            # print(s)
            s += "\n"
            # s = s.encode()
            fp.write(s)
    # with open(f'{DATAPATH}/result.txt','w') as fp:
    #     # fp.write(str(res))
    #     for operation in res:
    #         operation = str(operation) + "\n"
    #         fp.write(operation)
    