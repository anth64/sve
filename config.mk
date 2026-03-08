SRC_DIR  = src
INC_DIR  = include
OBJ_DIR  = obj
BIN_DIR  = bin

CLIENT_BIN = sve
SERVER_BIN = sve_server

STK_SRCS = \
    vendor/stk/src/stk.c \
    vendor/stk/src/stk_log.c \
    vendor/stk/src/module.c \
    vendor/stk/src/platform.c

ENGINE_SRCS = \
    src/sve.c \
    src/platform.c

CLIENT_SRCS = src/client/main.c
SERVER_SRCS = src/server/main.c
