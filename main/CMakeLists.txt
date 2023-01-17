
idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "include"
                       REQUIRES "test_mbedtls" "test_hal")

message(WARNING MAIN)
