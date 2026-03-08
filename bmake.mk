.include "config.mk"

UNAME_S != uname -s
.if ${UNAME_S} == "Darwin"
FULL_BIN     = ${BIN_NAME}
.else
FULL_BIN     = ${BIN_NAME}
.endif

LDFLAGS_PLAT =
CFLAGS_PLAT  =
CFLAGS_BASE  = -Wall -Wpedantic -I${.CURDIR}/${INC_DIR} -I${.CURDIR}/vendor/stk/include -std=c99 ${CFLAGS_PLAT}

.PHONY: all debug release clean
all: debug

OBJS_DEBUG   = ${SRCS:S/^src\//obj\/debug\/src\//:S/^vendor\//obj\/debug\/vendor\//:S/.c$/.o/}
OBJS_RELEASE = ${SRCS:S/^src\//obj\/release\/src\//:S/^vendor\//obj\/release\/vendor\//:S/.c$/.o/}

debug: ${BIN_DIR}/debug/${FULL_BIN}
release: ${BIN_DIR}/release/${FULL_BIN}

${BIN_DIR}/debug/${FULL_BIN}: ${OBJS_DEBUG}
	@mkdir -p ${.TARGET:H}
	${CC} -o ${.TARGET} ${.ALLSRC} ${LDFLAGS_PLAT}

${BIN_DIR}/release/${FULL_BIN}: ${OBJS_RELEASE}
	@mkdir -p ${.CURDIR}/${BIN_DIR}/release
	${CC} -s -o ${.CURDIR}/${BIN_DIR}/release/${FULL_BIN} ${.ALLSRC} ${LDFLAGS_PLAT}

.for _src in ${SRCS}
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
