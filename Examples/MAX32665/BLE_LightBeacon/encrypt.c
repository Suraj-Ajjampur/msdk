#include "encrypt.h"

/***** Globals *****/
char temp[] = { 0x00, 0x00, 0x00 }; //for ascii_to_byte()
char result[16];
const char *_key = "10a58869d74be5a374cf867cfb473859";
char outresult[16];

void ascii_to_byte(const char *src, char *dst, int len)
{
    int i;
    for (i = 0; i < len; ++i) {
        int val;
        temp[0] = *src;
        src++;
        temp[1] = *src;
        src++;
        sscanf(temp, "%x", &val);
        //val = hexconf(temp[0])*16 + hexconf(temp[1]);
        dst[i] = val;
    }
    return;
}

char* AES128_ECB_enc(char* inputData)
{
    //printf("128-bit AES ECB Encryption ... ");
    char key[MXC_AES_KEY_128_LEN];
    ascii_to_byte(_key, key, MXC_AES_KEY_128_LEN);

    const char *iv_src = "";
    char iv_dst[16];
    ascii_to_byte(iv_src, iv_dst, 16);

    //const char *_msg = "00000000000000000000000000000000";
    char msg[MXC_AES_DATA_LEN];
    ascii_to_byte(inputData, msg, MXC_AES_DATA_LEN);

    MXC_TPU_Cipher_Config(MXC_TPU_MODE_ECB, MXC_TPU_CIPHER_AES128);
    MXC_TPU_Cipher_AES_Encrypt(msg, iv_dst, key, MXC_TPU_CIPHER_AES128, MXC_TPU_MODE_ECB,
                               MXC_AES_DATA_LEN, result);
    return result;
}

char* AES128_ECB_dec(char* inputData)
{
    char key[MXC_AES_KEY_128_LEN];
    ascii_to_byte(_key, key, MXC_AES_KEY_128_LEN);

    const char *iv_src = "";
    char iv_dst[16];
    ascii_to_byte(iv_src, iv_dst, 16);

    // const char *_ct = "6d251e6944b051e04eaa6fb4dbf78465";
    char ct[MXC_AES_DATA_LEN];
    ascii_to_byte(inputData, ct, MXC_AES_DATA_LEN);

    //char expected[MXC_AES_DATA_LEN];
    MXC_TPU_Cipher_Config(MXC_TPU_MODE_ECB, MXC_TPU_CIPHER_AES128);
    MXC_TPU_Cipher_AES_Decrypt(inputData, iv_dst, key, MXC_TPU_CIPHER_AES128, MXC_TPU_MODE_ECB,
                               MXC_AES_DATA_LEN, outresult);

    //const char *_expected = "00000000000000000000000000000000";
    
    //ascii_to_byte(_expected, expected, MXC_AES_DATA_LEN);

    return outresult;
}