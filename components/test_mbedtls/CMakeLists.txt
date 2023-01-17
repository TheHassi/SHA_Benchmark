idf_component_register(SRCS "test_mbedtls.c"
                       INCLUDE_DIRS "include"
                       REQUIRES mbedtls perfmon xtensa)

message(WARNING TEST_MBEDTLS)