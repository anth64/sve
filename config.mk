SRC_DIR  = src
INC_DIR  = include
OBJ_DIR  = obj
BIN_DIR  = bin

CLIENT_BIN = sve
SERVER_BIN = sve_server

ENGINE_SRCS = \
    src/sve.c \
    src/platform.c

CLIENT_SRCS = src/client/main.c
SERVER_SRCS = src/server/main.c
