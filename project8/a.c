#include <stdio.h>
#include <string.h>
#include <arm_neon.h>

void aes_encrypt(const uint8_t* plaintext, const uint8_t* key, uint8_t* ciphertext) {
    uint8x16_t m = vld1q_u8(plaintext);
    uint8x16_t k = vld1q_u8(key);

    m = vaeseq_u8(m, k);
    m = vaesmcq_u8(m);
    m = vaeseq_u8(m, k);
    m = vaesmcq_u8(m);
    m = vaeseq_u8(m, k);
    m = vaesmcq_u8(m);
    m = vaeseq_u8(m, k);
    m = vaesmcq_u8(m);
    m = vaeseq_u8(m, k);
    m = vaesmcq_u8(m);
    m = vaeseq_u8(m, k);
    m = vaesmcq_u8(m);
    m = vaeseq_u8(m, k);
    m = vaesmcq_u8(m);
    m = vaeseq_u8(m, k);

    vst1q_u8(ciphertext, m);
}

void print_hex(const uint8_t* data, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

int main() {
    // Input data and key
    uint8_t plaintext[] = "HelloWorld";
    uint8_t key[] = {0x06, 0xa9, 0x21, 0x40, 0x36, 0xb8, 0xa1, 0x5b,
    0x51, 0x2e, 0x03, 0xd5, 0x34, 0x12, 0x00, 0x06};

    // Determine the size of the ciphertext based on the plaintext length
    int plaintext_len = strlen((const char*)plaintext);
    int ciphertext_len = (plaintext_len / 16 + 1) * 16;

    // Perform AES encryption
    uint8_t ciphertext[ciphertext_len];
    aes_encrypt(plaintext, key, ciphertext);

    // Print ciphertext
    printf("Ciphertext: \n");
    print_hex(ciphertext, ciphertext_len);
printf("\n");
    return 0;
}

