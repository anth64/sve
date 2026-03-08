.include "config.mk"

UNAME_S != uname -s
.if ${UNAME_S} == "Darwin"
CLIENT_FULL_BIN = ${CLIENT_BIN}
SERVER_FULL_BIN = ${SERVER_BIN}
.else
CLIENT_FULL_BIN = ${CLIENT_BIN}
SERVER_FULL_BIN = ${SERVER_BIN}
.endif

LDFLAGS_PLAT =
CFLAGS_PLAT  =
CFLAGS_BASE  = -Wall -Wpedantic -I${.CURDIR}/${INC_DIR} -I${.CURDIR}/vendor/stk/include -I${.CURDIR}/vendor/stk/include/stk -std=c99 ${CFLAGS_PLAT}

ALL_CLIENT_SRCS = ${CLIENT_SRCS} ${ENGINE_SRCS} ${STK_SRCS}
ALL_SERVER_SRCS = ${SERVER_SRCS} ${ENGINE_SRCS} ${STK_SRCS}

# All unique sources combined (no duplicates)
ALL_SRCS = ${CLIENT_SRCS} ${SERVER_SRCS} ${ENGINE_SRCS} ${STK_SRCS}

CLIENT_DEBUG_OBJS   = ${ALL_CLIENT_SRCS:S/.c$/.o/:S/^/obj\/debug\//}
CLIENT_RELEASE_OBJS = ${ALL_CLIENT_SRCS:S/.c$/.o/:S/^/obj\/release\//}
SERVER_DEBUG_OBJS   = ${ALL_SERVER_SRCS:S/.c$/.o/:S/^/obj\/debug\//}
SERVER_RELEASE_OBJS = ${ALL_SERVER_SRCS:S/.c$/.o/:S/^/obj\/release\//}

.PHONY: all debug release client server clean
all: debug

debug: ${BIN_DIR}/debug/${CLIENT_FULL_BIN} ${BIN_DIR}/debug/${SERVER_FULL_BIN}
release: ${BIN_DIR}/release/${CLIENT_FULL_BIN} ${BIN_DIR}/release/${SERVER_FULL_BIN}
client: ${BIN_DIR}/debug/${CLIENT_FULL_BIN}
server: ${BIN_DIR}/debug/${SERVER_FULL_BIN}

${BIN_DIR}/debug/${CLIENT_FULL_BIN}: ${CLIENT_DEBUG_OBJS}
	@mkdir -p ${.TARGET:H}
	${CC} -o ${.TARGET} ${.ALLSRC} ${LDFLAGS_PLAT}

${BIN_DIR}/debug/${SERVER_FULL_BIN}: ${SERVER_DEBUG_OBJS}
	@mkdir -p ${.TARGET:H}
	${CC} -o ${.TARGET} ${.ALLSRC} ${LDFLAGS_PLAT}

${BIN_DIR}/release/${CLIENT_FULL_BIN}: ${CLIENT_RELEASE_OBJS}
	@mkdir -p ${.TARGET:H}
	${CC} -s -o ${.TARGET} ${.ALLSRC} ${LDFLAGS_PLAT}

${BIN_DIR}/release/${SERVER_FULL_BIN}: ${SERVER_RELEASE_OBJS}
	@mkdir -p ${.TARGET:H}
	${CC} -s -o ${.TARGET} ${.ALLSRC} ${LDFLAGS_PLAT}

.for _src in ${ALL_SRCS}
_obj_base = ${_src:S/.c$/.o/}
obj/debug/${_obj_base}: ${_src}
	@mkdir -p ${.TARGET:H}
	${CC} ${CFLAGS_BASE} -g -O0 -MMD -MP -c ${.ALLSRC} -o ${.TARGET}
obj/release/${_obj_base}: ${_src}
	@mkdir -p ${.TARGET:H}
	${CC} ${CFLAGS_BASE} -O2 -MMD -MP -c ${.ALLSRC} -o ${.TARGET}
.endfor

.-include "obj/debug/*.d"
.-include "obj/release/*.d"

clean:
	rm -rf ${.CURDIR}/${OBJ_DIR} ${.CURDIR}/${BIN_DIR}
