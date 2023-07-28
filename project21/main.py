import hashlib
import random

# 椭圆曲线参数（secp256k1）
p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
a = 0
b = 7
Gx = 0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798
Gy = 0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8
n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141


def generate_key_pair() :
    # 生成随机私钥
    private_key = random.randint ( 1, n - 1 )

    # 计算公钥
    public_key = multiply ( private_key, (Gx, Gy) )

    return private_key, public_key


def multiply(k, P) :
    # 执行椭圆曲线上的点乘法操作
    Q = None

    for bit in bin ( k )[2 :] :
        Q = double ( Q )

        if bit == "1" :
            Q = add ( Q, P )

    return Q


def double(P) :
    if P is None :
        return None

    x, y = P

    # 计算斜率
    s = (3 * x ** 2) * pow ( 2 * y, p - 2, p ) % p

    # 计算结果点的 x 坐标
    x3 = (s ** 2 - 2 * x) % p

    # 计算结果点的 y 坐标
    y3 = (s * (x - x3) - y) % p

    return x3, y3


def add(P, Q) :
    if P is None :
        return Q
    elif Q is None :
        return P

    x1, y1 = P
    x2, y2 = Q

    if x1 == x2 and y1 != y2 :
        return None

    if P == Q :
        return double ( P )

    # 计算斜率
    s = (y1 - y2) * pow ( x1 - x2, p - 2, p ) % p

    # 计算结果点的 x 坐标
    x3 = (s ** 2 - x1 - x2) % p

    # 计算结果点的 y 坐标
    y3 = (s * (x1 - x3) - y1) % p

    return x3, y3


def sign_message(private_key, message) :
    # 生成随机数 k (k < n)
    k = random.randint ( 1, n - 1 )

    # 计算 R = k * G （椭圆曲线上的点）
    R = multiply ( k, (Gx, Gy) )

    # 将 x 坐标转换为整数
    R = bytes_to_int ( R[0].to_bytes ( 32, 'big' ) )

    # 计算 e = H(R, M)
    e = int ( hashlib.sha256 ( R.to_bytes ( 32, 'big' ) + message.encode () ).hexdigest (), 16 )

    # 计算 s = k - e * private_key mod n
    s = (k - e * private_key) % n

    return R, s


def verify_signature(public_key, message, R, s) :
    # 计算 e = H(R, M)
    e = int ( hashlib.sha256 ( R.to_bytes ( 32, 'big' ) + message.encode () ).hexdigest (), 16 )

    # 计算 Q = s * G + e * public_key
    Q = add ( multiply ( s, (Gx, Gy) ), multiply ( e, public_key ) )

    # 将 x 坐标转换为整数
    Q = bytes_to_int ( Q[0].to_bytes ( 32, 'big' ) )

    # 验证 R == Q
    return R == Q


# 辅助函数：将字节数组转换为整数
def bytes_to_int(b) :
    return int.from_bytes ( b, 'big' )


# 生成密钥对
private_key, public_key = generate_key_pair ()
print ( "私钥: ", private_key )
print ( "公钥: ", public_key )

# 要签名的消息
message = "Hello, world!"

# 签名
R, s = sign_message ( private_key, message )
print ( "签名 R: ", hex ( R ) )
print ( "签名 s: ", hex ( s ) )

# 验证签名
valid = verify_signature ( public_key, message, R, s )
print ( "签名验证结果: ", valid )
