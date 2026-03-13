.include "config.mk"

UNAME_S != uname -s
.if ${UNAME_S} == "Darwin"
CLIENT_FULL_BIN = ${CLIENT_BIN}
SERVER_FULL_BIN = ${SERVER_BIN}
.else
CLIENT_FULL_BIN = ${CLIENT_BIN}
SERVER_FULL_BIN = ${SERVER_BIN}
.endif

LDFLAGS_PLAT = -L/usr/local/lib
CFLAGS_PLAT  = -I/usr/local/include
CFLAGS_BASE  = -Wall -Wpedantic -I${.CURDIR}/${INC_DIR} -std=c99 ${CFLAGS_PLAT}

ALL_CLIENT_SRCS = ${CLIENT_SRCS} ${ENGINE_SRCS}
ALL_SERVER_SRCS = ${SERVER_SRCS} ${ENGINE_SRCS}

ALL_SRCS = ${CLIENT_SRCS} ${SERVER_SRCS} ${ENGINE_SRCS}

CLIENT_DEBUG_OBJS   = ${ALL_CLIENT_SRCS:S/.c$/.o/:S|^|${.CURDIR}/obj/debug/|}
CLIENT_RELEASE_OBJS = ${ALL_CLIENT_SRCS:S/.c$/.o/:S|^|${.CURDIR}/obj/release/|}
SERVER_DEBUG_OBJS   = ${ALL_SERVER_SRCS:S/.c$/.o/:S|^|${.CURDIR}/obj/debug/|}
SERVER_RELEASE_OBJS = ${ALL_SERVER_SRCS:S/.c$/.o/:S|^|${.CURDIR}/obj/release/|}

PREFIX ?= /usr/local

.PHONY: all debug release client server install uninstall clean
all: debug

debug: ${.CURDIR}/${BIN_DIR}/debug/${CLIENT_FULL_BIN} ${.CURDIR}/${BIN_DIR}/debug/${SERVER_FULL_BIN}
release: ${.CURDIR}/${BIN_DIR}/release/${CLIENT_FULL_BIN} ${.CURDIR}/${BIN_DIR}/release/${SERVER_FULL_BIN}
client: ${.CURDIR}/${BIN_DIR}/debug/${CLIENT_FULL_BIN}
server: ${.CURDIR}/${BIN_DIR}/debug/${SERVER_FULL_BIN}

${.CURDIR}/${BIN_DIR}/debug/${CLIENT_FULL_BIN}: ${CLIENT_DEBUG_OBJS}
	@mkdir -p ${.TARGET:H}
	${CC} -o ${.TARGET} ${.ALLSRC} ${LDFLAGS_CLIENT} ${LDFLAGS_PLAT}

${.CURDIR}/${BIN_DIR}/debug/${SERVER_FULL_BIN}: ${SERVER_DEBUG_OBJS}
	@mkdir -p ${.TARGET:H}
	${CC} -o ${.TARGET} ${.ALLSRC} ${LDFLAGS_SERVER} ${LDFLAGS_PLAT}

${.CURDIR}/${BIN_DIR}/release/${CLIENT_FULL_BIN}: ${CLIENT_RELEASE_OBJS}
	@mkdir -p ${.TARGET:H}
	${CC} -s -o ${.TARGET} ${.ALLSRC} ${LDFLAGS_CLIENT} ${LDFLAGS_PLAT}

${.CURDIR}/${BIN_DIR}/release/${SERVER_FULL_BIN}: ${SERVER_RELEASE_OBJS}
	@mkdir -p ${.TARGET:H}
	${CC} -s -o ${.TARGET} ${.ALLSRC} ${LDFLAGS_SERVER} ${LDFLAGS_PLAT}

.for _src in ${ALL_SRCS}
${.CURDIR}/obj/debug/${_src:S/.c$/.o/}: ${.CURDIR}/${_src}
	@mkdir -p ${.TARGET:H}
	${CC} ${CFLAGS_BASE} -g -O0 -MMD -MP -MT ${.TARGET} -MF ${.TARGET:S/.o$/.d/} -c ${.ALLSRC} -o ${.TARGET}
${.CURDIR}/obj/release/${_src:S/.c$/.o/}: ${.CURDIR}/${_src}
	@mkdir -p ${.TARGET:H}
	${CC} ${CFLAGS_BASE} -O2 -MMD -MP -MT ${.TARGET} -MF ${.TARGET:S/.o$/.d/} -c ${.ALLSRC} -o ${.TARGET}
.endfor

install:
	install -d ${PREFIX}/bin
	install -m 755 ${.CURDIR}/${BIN_DIR}/release/${CLIENT_FULL_BIN} ${PREFIX}/bin/${CLIENT_FULL_BIN}
	install -m 755 ${.CURDIR}/${BIN_DIR}/release/${SERVER_FULL_BIN} ${PREFIX}/bin/${SERVER_FULL_BIN}

uninstall:
	rm -f ${PREFIX}/bin/${CLIENT_FULL_BIN}
	rm -f ${PREFIX}/bin/${SERVER_FULL_BIN}

clean:
	rm -rf ${.CURDIR}/${OBJ_DIR} ${.CURDIR}/${BIN_DIR}
