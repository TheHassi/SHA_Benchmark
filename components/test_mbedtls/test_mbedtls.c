#include "include/test_mbedtls.h"

struct timeval hash_mbedtls_gettimeofday(int sha_type, char *payload, size_t payload_len, long repeats, int garbage)
{
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = sha_type;
    unsigned char SHA_result[32];
    struct timeval tval_before, tval_after, tval_sub, tval_result, tval_fault;
    tval_fault.tv_sec = 0;
    tval_fault.tv_usec = 0;
    tval_result.tv_sec = 0;
    tval_result.tv_usec = 0;

    // gettimeofday Fehler
    for (int i = 0; i < 1000; i++)
    {
        gettimeofday(&tval_before, NULL);
        gettimeofday(&tval_after, NULL);
        if (i == 0)
        {
            continue;
        }
        timersub(&tval_after, &tval_before, &tval_sub);
        timeradd(&tval_sub, &tval_fault, &tval_fault);
    }
    tval_fault.tv_sec /= 1000;
    tval_fault.tv_usec /= 1000;

    for (long i = 0; i < repeats + 1; i++)
    {
        mbedtls_md_init(&ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);

        gettimeofday(&tval_before, NULL);
        mbedtls_md_starts(&ctx);
        mbedtls_md_update(&ctx, (const unsigned char *)payload, payload_len);
        mbedtls_md_finish(&ctx, SHA_result);
        gettimeofday(&tval_after, NULL);
        timersub(&tval_after, &tval_before, &tval_sub);
        if (i <= garbage)
        {
            mbedtls_md_free(&ctx);
            continue;
        }

        timeradd(&tval_sub, &tval_result, &tval_result);
        timersub(&tval_result, &tval_fault, &tval_result); // Fault handling

        mbedtls_md_free(&ctx);

        // printf("Ergebnis: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
    }
/* 
    printf("mbedtls: ");
    for (int i = 0; i < sizeof(SHA_result); i++)
    {
        char str[3];
        sprintf(str, "%02x", (int)SHA_result[i]);
        printf("%s", str);
    }
    printf("\n"); 
 */
    return tval_result;
}

double hash_mbedtls_clock(int sha_type, char *payload, size_t payload_len, long repeats, int garbage)
{
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = sha_type;
    unsigned char SHA_result[32];

    struct timespec start, stop;
    double sub, result, fault;
    fault = 0;
    result = 0;

    for (int i = 0; i < 1000; i++)
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        clock_gettime(CLOCK_MONOTONIC, &stop);

        if (i == 0)
        {
            continue;
        }
        sub = (stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec) / (double)1000000000;
        fault += sub;
    }
    fault /= 1000;

    for (long i = 0; i < repeats + 1; i++)
    {
        mbedtls_md_init(&ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);

        clock_gettime(CLOCK_MONOTONIC, &start);
        mbedtls_md_starts(&ctx);
        mbedtls_md_update(&ctx, (const unsigned char *)payload, payload_len);
        mbedtls_md_finish(&ctx, SHA_result);
        clock_gettime(CLOCK_MONOTONIC, &stop);

        if (i <= garbage)
        {
            mbedtls_md_free(&ctx);
            continue;
        }

        sub = (stop.tv_sec - start.tv_sec) + (double)(stop.tv_nsec - start.tv_nsec) / (double)1000000000;
        result = result + sub - fault;

        mbedtls_md_free(&ctx);
    }

    /*
    printf("mbedtls: ");
    for (int i = 0; i < sizeof(SHA_result); i++)
    {
        char str[3];
        sprintf(str, "%02x", (int)SHA_result[i]);
        printf("%s", str);
    }
    printf("\n");
    */

    return result;
}

uint32_t hash_mbedtls_perfmon(int sha_type, char *payload, size_t payload_len, long repeats, int garbage)
{
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = sha_type;
    unsigned char SHA_result[32];

    if (xtensa_perfmon_init(0, 0, 0xffff, 1, 0) == ESP_ERR_INVALID_ARG)
    {
        printf("perfmon wrong initialized\n");
    }
    uint32_t result = 0;
    uint32_t fault = 0;
    uint32_t pre_result;

    for (int i = 0; i < 1000; i++)
    {
        xtensa_perfmon_stop();
        xtensa_perfmon_reset(0);
        xtensa_perfmon_start();
        fault += xtensa_perfmon_value(0);
    }

    fault = fault / 1000;
    result = 0;

    for (long i = 0; i < repeats + 1; i++)
    {
        mbedtls_md_init(&ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);

        xtensa_perfmon_stop();
        xtensa_perfmon_reset(0);
        xtensa_perfmon_start();
        mbedtls_md_starts(&ctx);
        mbedtls_md_update(&ctx, (const unsigned char *)payload, payload_len);
        mbedtls_md_finish(&ctx, SHA_result);
        pre_result = xtensa_perfmon_value(0);

        if (i <= garbage)
        {
            mbedtls_md_free(&ctx);
            continue;
        }

        result += pre_result - fault;
        /*
        printf("Taktanzahl: ");
        printf("%" PRIu32, result);
        printf("\n");
        */

        mbedtls_md_free(&ctx);
    }

    /*
    printf("mbedtls: ");
    for (int i = 0; i < sizeof(SHA_result); i++)
    {
        char str[3];
        sprintf(str, "%02x", (int)SHA_result[i]);
        printf("%s", str);
    }
    printf("\n");
    */

    return result;
}

uint32_t hash_mbedtls_CCOUNT(int sha_type, char *payload, size_t payload_len, long repeats, int garbage)
{
    mbedtls_md_context_t ctx;
    mbedtls_md_type_t md_type = sha_type;
    unsigned char SHA_result[32];

    uint32_t cc1, cc2, fault = 0, result = 0;

    for (int i = 0; i < 1000; i++)
    {
        RSR(CCOUNT, cc1);
        RSR(CCOUNT, cc2);
        fault += cc2 - cc1;
    }

    fault = fault / 1000;

    for (long i = 0; i < repeats + 1; i++)
    {
        mbedtls_md_init(&ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
        mbedtls_md_starts(&ctx);

        RSR(CCOUNT, cc1);
        mbedtls_md_update(&ctx, (const unsigned char *)payload, payload_len);
        mbedtls_md_finish(&ctx, SHA_result);
        RSR(CCOUNT, cc2);

        if (i <= garbage)
        {
            mbedtls_md_free(&ctx);
            continue;
        }

        result += cc2 - cc1 - fault;
        /*
        printf("Taktanzahl: ");
        printf("%" PRIu32, result);
        printf("\n");
        */

        mbedtls_md_free(&ctx);
    }

    /*
    printf("mbedtls: ");
    for (int i = 0; i < sizeof(SHA_result); i++)
    {
        char str[3];
        sprintf(str, "%02x", (int)SHA_result[i]);
        printf("%s", str);
    }
    printf("\n");
    */

    return result;
}

uint32_t hash_mbedtls_comp_CCOUNT(int sha_type, char *payload, size_t payload_len, long repeats, int garbage)
{
    uint32_t cc1, cc2, fault = 0, result = 0;

    for (int i = 0; i < 1000; i++)
    {
        RSR(CCOUNT, cc1);
        RSR(CCOUNT, cc2);
        fault += cc2 - cc1;
    }

    fault = fault / 1000;

    for (long i = 0; i < repeats + 1; i++)
    {
        RSR(CCOUNT, cc1);
        mbedtls_md_context_t ctx;
        mbedtls_md_type_t md_type = sha_type;
        unsigned char SHA_result[32];

        mbedtls_md_init(&ctx);
        mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);

        
        mbedtls_md_starts(&ctx);
        mbedtls_md_update(&ctx, (const unsigned char *)payload, payload_len);
        mbedtls_md_finish(&ctx, SHA_result);
        RSR(CCOUNT, cc2);

        if (i <= garbage)
        {
            mbedtls_md_free(&ctx);
            continue;
        }

        result += cc2 - cc1 - fault;
        /*
        printf("Taktanzahl: ");
        printf("%" PRIu32, result);
        printf("\n");
        */

        mbedtls_md_free(&ctx);
    }

    /*
    printf("mbedtls: ");
    for (int i = 0; i < sizeof(SHA_result); i++)
    {
        char str[3];
        sprintf(str, "%02x", (int)SHA_result[i]);
        printf("%s", str);
    }
    printf("\n");
    */

    return result;
}