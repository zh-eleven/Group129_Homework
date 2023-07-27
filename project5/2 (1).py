import hashlib

def calculate_hash(data):
    # 使用哈希函数计算给定数据的哈希值
    hash_object = hashlib.sha256()
    hash_object.update(data.encode('utf-8'))
    return hash_object.hexdigest()

def build_merkle_tree(data_blocks):
    # 如果数据块为空，返回空树
    if len(data_blocks) == 0:
        return []

    # 如果只有一个数据块，返回叶子节点
    if len(data_blocks) == 1:
        leaf_hash = calculate_hash(data_blocks[0])
        print("Leaf Node Hash:", leaf_hash)
        return [leaf_hash]

    # 构建上层节点
    parent_nodes = []
    for i in range(0, len(data_blocks), 2):
        node1 = calculate_hash(data_blocks[i])
        if i+1 < len(data_blocks):
            node2 = calculate_hash(data_blocks[i+1])
        else:
            node2 = node1
        parent_hash = calculate_hash(node1 + node2)
        print("Parent Node Hash:", parent_hash)
        parent_nodes.append(parent_hash)

    # 递归构建Merkle树
    return build_merkle_tree(parent_nodes)

data_blocks = ["Data Block 1", "Data Block 2", "Data Block 3", "Data Block 4"]

# 构建Merkle树
merkle_tree = build_merkle_tree(data_blocks)

# 打印Merkle树的根节点
print("Merkle Root:", merkle_tree[0])
