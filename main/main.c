#include "main.h"

void handleErrors()
{
    exit(EXIT_FAILURE);
}

void app_main(void)
{
    periph_module_enable(PERIPH_SHA_MODULE);

    /* 
    char *text = "Hello SHA 256 from ESP32learning";
    int size_mul = 15;
    size_t payload_len = strlen(text) * size_mul;
    char *payload = (char *)malloc(32 * size_mul + 1);
    for (int i = 0; i < payload_len; i += 32)
    {
        memcpy(payload + i, text, 32);
    }
    memcpy(payload + payload_len, "\0", 1);
 */
    char *payload = "Hello SHA 256 from ESP32learning";
    size_t payload_len = strlen(payload);

    // printf("Payload: %s\n", payload);
    printf("Size of Payload: %d\n", strlen(payload));

    // uint32_t CPU_frequenz = rtc_clk_cpu_freq_value(rtc_clk_cpu_freq_get());
    double CPU_frequenz = 80000000;
    printf("CPU-Frequenz: %lu\n", (unsigned long)CPU_frequenz);

    long repeats = 1000;
    int garbage = 5;
    double per_cycle;
    double takte;
    uint32_t uint32_takte;
    /*
    // use mbedtls & gettimeofday()
    printf("\nmbedtls & gettimeofday()\n");
    struct timeval tval_result = hash_mbedtls_gettimeofday(MBEDTLS_MD_SHA256, payload, payload_len, repeats, garbage);
    // printf("Ergebnis: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    per_cycle = ((double)tval_result.tv_sec + ((double)tval_result.tv_usec) / 1000000) / (repeats - garbage);
    // printf("Ergebnis pro Zyklus: %f\n", per_cycle);
    takte = per_cycle * CPU_frequenz;
    printf("Taktanzahl: %f\n", takte);

    // use mbedtls & clock_gettime()
    printf("\nmbedtls & clock_gettime()\n");
    double clock_result = hash_mbedtls_clock(MBEDTLS_MD_SHA256, payload, payload_len, repeats, garbage);
    // printf("Ergebnis: %lf\n", clock_result);
    per_cycle = clock_result / (repeats - garbage);
    // printf("Ergebnis pro Zyklus: %f\n", per_cycle);
    takte = per_cycle * CPU_frequenz;
    printf("Taktanzahl: %f\n", takte);

    // use mbedtls & component perfmon
    printf("\nmbedtls & component perfmon\n");
    uint32_takte = hash_mbedtls_perfmon(MBEDTLS_MD_SHA256, payload, payload_len, repeats, garbage);
    takte = (double)uint32_takte / ((double)(repeats - garbage));
    printf("Taktanzahl: %f\n", takte);

    // use mbedtls & component CCOUNT
    printf("\nmbedtls & component CCOUNT\n");
    uint32_takte = hash_mbedtls_CCOUNT(MBEDTLS_MD_SHA256, payload, payload_len, repeats, garbage);
    takte = (double)uint32_takte / ((double)(repeats - garbage));
    printf("Taktanzahl: %f\n", takte);

    // use mbedtls & component CCOUNT complete
    printf("\nmbedtls & component complete CCOUNT\n");
    uint32_takte = hash_mbedtls_comp_CCOUNT(MBEDTLS_MD_SHA256, payload, payload_len, repeats, garbage);
    takte = (double)uint32_takte / ((double)(repeats - garbage));
    printf("Taktanzahl: %f\n", takte);
    */

    // hash_hal(SHA2_256, payload, payload_len);
    uint32_takte = hash_hal_comp(SHA2_256, payload, payload_len, repeats, garbage);
    takte = (double)uint32_takte / ((double)(repeats - garbage));
    printf("Taktanzahl: %f\n", takte);
    printf("DONE\n");
}
