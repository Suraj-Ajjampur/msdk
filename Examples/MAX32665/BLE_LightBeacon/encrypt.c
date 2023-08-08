/**************************************************************************************************
  Include Files
**************************************************************************************************/
#include "encrypt.h"

/**************************************************************************************************
  Global Variables
**************************************************************************************************/
char temp[] = { 0x00, 0x00, 0x00 }; //for ascii_to_byte()
char result[16];
const char *_key = "10a58869d74be5a374cf867cfb473859";

/**************************************************************************************************
  Function Definitions
**************************************************************************************************/
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
        dst[i] = val;
    }
    return;
}

char* AES128_ECB_enc(char* inputData)
{
    char key[MXC_AES_KEY_128_LEN];
    ascii_to_byte(_key, key, MXC_AES_KEY_128_LEN);

    const char *iv_src = "";
    char iv_dst[16];
    ascii_to_byte(iv_src, iv_dst, 16);

    MXC_TPU_Cipher_Config(MXC_TPU_MODE_ECB, MXC_TPU_CIPHER_AES128);
    MXC_TPU_Cipher_AES_Encrypt(inputData, iv_dst, key, MXC_TPU_CIPHER_AES128, MXC_TPU_MODE_ECB,
                               MXC_AES_DATA_LEN, result);
    return result;
}
