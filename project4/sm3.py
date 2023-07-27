import hashlib
import datetime
import time
def sm3_hash(ciphertext):
    m = hashlib.new('sm3')
    m.update(ciphertext)
    return m.hexdigest()

# 密文数据
ciphertext = b'\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64\x61\x62\x63\x64'    # 64 bytes

# 计算 SM3 哈希值
start_time = time.time()
for i in range(10000):
    hash_value = sm3_hash(ciphertext)
end_time = time.time()

print("SM3 hash:", hash_value)
print("time cost:", float(end_time - start_time) * 1.0, "s")