#include "decrypt.h"

/***** Globals *****/
char outresult[16];
uint8_t _key[16] = {0x10, 0xa5, 0x88, 0x69, 0xd7, 0x4b, 0xe5, 0xa3, 0x74, 0xcf, 0x86, 0x7c, 0xfb, 0x47, 0x38, 0x59};


char* AES128_ECB_dec(char* inputData)
{
    const char *iv_src = "";

    MXC_TPU_Cipher_Config(MXC_TPU_MODE_ECB, MXC_TPU_CIPHER_AES128);
    MXC_TPU_Cipher_AES_Decrypt(inputData, iv_src, (char *)_key, MXC_TPU_CIPHER_AES128, MXC_TPU_MODE_ECB,
                               MXC_AES_DATA_LEN, outresult);

    return outresult;
}