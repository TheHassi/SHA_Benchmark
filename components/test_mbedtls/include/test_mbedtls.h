#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include "mbedtls/md.h"
#include "perfmon.h"
#include "esp_cpu.h"

// Finktionsdeklaration
struct timeval hash_mbedtls_gettimeofday(int sha_type, char *payload, size_t payload_len, long repeats, int garbage);
double hash_mbedtls_clock(int sha_type, char *payload, size_t payload_len, long repeats, int garbage);
uint32_t hash_mbedtls_perfmon(int sha_type, char *payload, size_t payload_len, long repeats, int garbage);
uint32_t hash_mbedtls_CCOUNT(int sha_type, char *payload, size_t payload_len, long repeats, int garbage);