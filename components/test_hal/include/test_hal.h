#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "esp_cpu.h"
#include "hal/sha_hal.h"
#include "hal/sha_ll.h"

void print_byte_as_bits(char val);
void print_bits(char *ty, char *val, unsigned char *bytes, size_t num_bytes);
void print_void_bytes(void *input_text, size_t input_text_len);
void hash_hal(esp_sha_type sha_type, char *payload, size_t payload_len);