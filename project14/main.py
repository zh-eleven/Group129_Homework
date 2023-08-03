import math
import random
from Crypto.Cipher import AES
from gmssl import sm3, func,sm2


def SM2_Mulyipoint(k, P, a, p):  # 返回值R = k*P , P为椭圆曲线上的一点 , k为正整数
    k_b = bin(k).replace('0b', '')  # 按2^i分层逐层运算
    i = len(k_b) - 1
    R = P
    if i > 0:
        k = k - 2 ** i
        while i > 0:
            R = SM2_Pluspoint(R, R, a, p)
            i -= 1
        if k > 0:
            R = SM2_Pluspoint(R, SM2_Mulyipoint(k, P, a, p), a, p)
    return R


def SM2_Pluspoint(P, Q, a, p):  # 返回值R = P+Q , P、Q为椭圆曲线上的两点 , 加法运算为定义在椭圆曲线上的加法
    if (math.isinf(P[0]) or math.isinf(P[1])) and (~math.isinf(Q[0]) and ~math.isinf(Q[1])):  # OP = P
        R = Q
    elif (~math.isinf(P[0]) and ~math.isinf(P[1])) and (math.isinf(Q[0]) or math.isinf(Q[1])):  # PO = P
        R = P
    elif (math.isinf(P[0]) or math.isinf(P[1])) and (math.isinf(Q[0]) or math.isinf(Q[1])):  # OO = O
        R = [float('inf'), float('inf')]
    else:
        if P != Q:
            l = SM2__Mod_Decimal(Q[1] - P[1], Q[0] - P[0], p)
        else:
            l = SM2__Mod_Decimal(3 * P[0] ** 2 + a, 2 * P[1], p)
        x = SM2_Mod(l ** 2 - P[0] - Q[0], p)
        y = SM2_Mod(l * (P[0] - x) - P[1], p)
        R = [x, y]
    return R


def SM2_Mod(a, b):  # 摸运算
    if math.isinf(a):
        return float('inf')
    else:
        return a % b


def SM2__Mod_Decimal(n, d, b):  # 返回值x = n*d^{-1} mod b
    if d == 0:
        x = float('inf')
    elif n == 0:
        x = 0
    else:
        a = bin(b - 2).replace('0b', '')
        y = 1
        i = 0
        while i < len(a):  # n/d = x mod b => x = n*d^(b-2) mod b
            y = (y ** 2) % b  # 快速指数运算
            if a[i] == '1':
                y = (y * d) % b
            i += 1
        x = (y * n) % b
    return x


def key_gen(a, p, n, G):  # SM2密钥对的生成

    sk = random.randint(1, n - 2)
    pk = SM2_Mulyipoint(sk, G, a, p)
    return sk, pk


def Legendre(n, p):  # 欧拉准则判断是否为二次剩余
    return pow(n, (p - 1) // 2, p)


def Tonelli_Shanks(n, p):  # Tonelli-Shanks算法求二次剩余
    assert Legendre(n, p) == 1
    if p % 4 == 3:
        return pow(n, (p + 1) // 4, p)
    q = p - 1
    s = 0
    while q % 2 == 0:
        q = q // 2
        s += 1
    for z in range(2, p):
        if Legendre(z, p) == p - 1:
            c = pow(z, q, p)
            break
    r = pow(n, (q + 1) // 2, p)
    t = pow(n, q, p)
    m = s
    if t % p == 1:
        return r
    else:
        i = 0
        while t % p != 1:
            temp = pow(t, 2 ** (i + 1), p)
            i += 1
            if temp % p == 1:
                b = pow(c, 2 ** (m - i - 1), p)
                r = r * b % p
                c = b * b % p
                t = t * c % p
                m = i
                i = 0
        return r


def MultiSetHash(sett):  # 定义集合的哈希 sett: bytes型集合
    digest_value = [float("inf"), float("inf")]
    for i in sett:
        x = int(sm3.sm3_hash(func.bytes_to_list(i)), 16)
        temp = SM2_Mod(x ** 2 + a * x + b, p)
        y = Tonelli_Shanks(temp, p)
        digest_value = SM2_Pluspoint(digest_value, [x, y], a, p)
    return digest_value


def SM2_enc(plaintext):
    ciphertext = sm2_crypt.encrypt(plaintext)
    return ciphertext


# SM2 decryption
def SM2_dec(ciphertext):
    plaintext = sm2_crypt.decrypt(ciphertext)
    return plaintext


def PGP_Encrypt(mes, k):
    mode = AES.MODE_OFB
    iv = b'0000000000000000'
    cryptor = AES.new(k.encode('utf-8'), mode, iv)
    length = 16
    count = len(mes)
    if count % length != 0:
        add = length - (count % length)
    else:
        add = 0
    mes = mes + ('\0' * add)
    ciphertext1 = cryptor.encrypt(mes.encode('utf-8'))
    plaintext_bytes = k.encode('utf-8')
    ciphertext2 = SM2_enc(plaintext_bytes)
    print("用会话密钥加密的消息值：", ciphertext1)
    print("用SM2公钥得到会话密钥的加密结果：", ciphertext2)
    return ciphertext1, ciphertext2


def PGP_Decrypt(mes1, mes2):
    mode = AES.MODE_OFB
    iv = b'0000000000000000'
    get_key = SM2_dec(mes2)
    print("用SM2私钥得到会话密钥：", get_key.decode('utf-8'))
    cryptor = AES.new(get_key, mode, iv)
    plain_text = cryptor.decrypt(mes1)
    print("原消息值", plain_text.decode('utf-8'))

if __name__ == '__main__':
    p = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
    a = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
    b = 0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
    n = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
    Gx = 0x32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7
    Gy = 0xBC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0
    G = [Gx, Gy]
    [sk, pk] = key_gen(a, p, n, G)
    sk_bytes = hex(sk)[2:]
    pk_bytes = hex(pk[0])[2:] + hex(pk[1])[2:]

    sm2_crypt = sm2.CryptSM2(public_key=pk_bytes, private_key=sk_bytes)
    msg = "202100460142"
    print("消息：", msg)
    k = hex(random.randint(2 ** 127, 2 ** 128))[2:]
    print("会话密钥：", k)
    result1, result2 = PGP_Encrypt(msg, k)
    PGP_Decrypt(result1, result2)