import hashlib
import secrets
import datetime
import time
def sm3(ciphertext):
    m = hashlib.new('sm3')
    m.update(ciphertext.encode('utf-8'))
    return m.hexdigest()

def chain(s):
    temp=s
    lst=[]
    lst.append(temp)
    for i in range(3):
        temp=sm3(temp)
        lst.append(temp)
    return lst

def andom_hex(length):
    num_bytes = length // 8  # 将比特转换为字节
    random_bytes = secrets.token_bytes(num_bytes)
    random_hex = random_bytes.hex()
    return random_hex

def hashChain(a,b,c):
    #a,b,c是发行的数字
    a1=int(a%10)
    a2=int((a%100)/10)
    a3=int(a/100)
    b1=int(b%10)
    b2=int((b%100)/10)
    b3=int(b/1000)
    c1=int(c%10)
    c2=int((c%100)/10)
    c3=int(c/1000)
    #生成哈希链
    s1 = andom_hex(256)
    s2 =  andom_hex(256)
    s3 =  andom_hex(256)
    s1_chain=chain(s1)
    s2_chain=chain(s2)
    s3_chain=chain(s3)
    temp1=s1_chain[3]+s2_chain[1]+s3_chain[2]
    temp2=s1_chain[3]+s2_chain[0]+s3_chain[3]
    temp3=s1_chain[2]+s2_chain[3]+s3_chain[3]
    temp1=sm3(temp1)
    temp2=sm3(temp2)
    temp3=sm3(temp3)
    return temp1,temp2,temp3


def build_merkle_tree(data_blocks):
    # 如果数据块为空，返回空树
    if len(data_blocks) == 0:
        return []

    # 如果只有一个数据块，返回叶子节点
    if len(data_blocks) == 1:
        leaf_hash = sm3(data_blocks[0])
        print("Leaf Node Hash:", leaf_hash)
        return [leaf_hash]

    # 构建上层节点
    parent_nodes = []
    for i in range(0, len(data_blocks), 2):
        node1 = sm3(data_blocks[i])
        if i+1 < len(data_blocks):
            node2 = sm3(data_blocks[i+1])
        else:
            node2 = node1
        parent_hash =sm3(node1 + node2)
        print("Node Hash:", parent_hash)
        parent_nodes.append(parent_hash)

    # 递归构建Merkle树
    return build_merkle_tree(parent_nodes)

def shuffle(a,b,c):
    #加盐
    salt1 = andom_hex(256)
    salt2 = andom_hex(256)
    salt3 = andom_hex(256)
    temp1 = sm3(salt1 + a)
    temp2 = sm3(salt2 + b)
    temp3 = sm3(salt3 + c)
    seed_D=andom_hex(256)
    shuffle_seed = andom_hex ( 256 )
    for i in range(9):
        seed_D=sm3(seed_D)
    lst=[]
    #padding
    for i in range(15):
        lst.append(andom_hex(256))
    lst[1] = temp1
    lst[2] = temp2
    lst[8] = temp3
    lst[9] =seed_D
    build_merkle_tree ( lst )
a,b,c=hashChain(312,303,233)
shuffle(a,b,c)