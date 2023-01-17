#include "test_hal.h"

#define SHOW(T, V)                                          \
    do                                                      \
    {                                                       \
        T x = V;                                            \
        print_bits(#T, #V, (unsigned char *)&x, sizeof(x)); \
    } while (0)

void print_byte_as_bits(char val)
{
    for (int i = 7; 0 <= i; i--)
    {
        printf("%c", (val & (1 << i)) ? '1' : '0');
    }
}

void print_bits(char *ty, char *val, unsigned char *bytes, size_t num_bytes)
{
    printf("(%*s) %*s = [ ", 15, ty, 16, val);
    for (size_t i = 0; i < num_bytes; i++)
    {
        print_byte_as_bits(bytes[i]);
        printf(" ");
    }
    printf("] %s \n", bytes);
}

void print_void_bytes(void *input_text, size_t input_text_len)
{
    for (int i = 0; i < input_text_len; i++)
    {
        printf("Byte: %02d Hex: %02x Char: %c\n", i, ((unsigned char *)input_text)[i], ((unsigned char *)input_text)[i]);
    }
}

void hash_hal(esp_sha_type sha_type, char *payload, size_t payload_len)
{
    unsigned char SHA_result[32];
    uint32_t cc1, cc2, fault = 0, result = 0;

    // SHA Hashing with component
    size_t input_text_len = 512 / 8;
    void *input_text = (void *)malloc(input_text_len);
    size_t digest_word_len = 256 / 8;
    void *digest_state = (void *)malloc(digest_word_len + 500);

    // Perform the padding
    int64_t l = payload_len * 8;
    int k = 448 - (l + 1) % 512;
    int n = (k + 1) / 8;
    size_t buf_len = payload_len + n + 8;
    unsigned char buf[buf_len];

    /*  // Prepare buf for SHA
     memcpy(buf, payload, payload_len);
     buf[payload_len] = 128;
     for (int i = payload_len + 1; i < payload_len + n + 7; i++)
     {
         buf[i] = 0;
     }
     memcpy(buf + sizeof(buf) - 8, &l, 8);

     // Printing Bits
     for (int j = 0; j < sizeof(buf); j++)
     {
         printf("%d: ", j);
         SHOW(unsigned char, buf[j]);
     } */

    // test different buffing
    int8_t x = 128;
    int8_t y = 0;
    memcpy(input_text, payload, payload_len);
    memcpy(input_text + payload_len, &x, 1);
    for (int i = payload_len + 1; i < payload_len + n + 7; i++)
    {
        // printf("%d", i);
        memcpy(input_text + i, &y, 1);
    }
    memcpy(input_text + input_text_len - 1, &l, 1);
    print_void_bytes(input_text, input_text_len);
    // printf("l in bits\n");
    print_void_bytes(&l, 8);

    // Start Hashing

    sha_hal_hash_block(SHA2_256, buf, buf_len, true);
    RSR(CCOUNT, cc1);
    sha_hal_wait_idle();
    RSR(CCOUNT, cc2);
    printf("Taktanzahl: ");
    printf("%" PRIu32, cc2 - cc1);
    printf("\n");
    sha_hal_read_digest(SHA2_256, digest_state);
    // Ausgabe
    // print_void_bytes(digest_state, digest_word_len);

    memcpy(SHA_result, digest_state, 32);
    printf("hal:     ");
    for (int i = 0; i < sizeof(SHA_result); i++)
    {
        char str[3];
        sprintf(str, "%02x", (int)SHA_result[i]);
        printf("%s", str);
    }
    printf("\n");
}