include config.mk

ifeq ($(OS),Windows_NT)
    SHELL           := cmd.exe
    CLIENT_FULL_BIN := $(CLIENT_BIN).exe
    SERVER_FULL_BIN := $(SERVER_BIN).exe
    LDFLAGS_PLAT    :=
    CFLAGS_PLAT     :=
    MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
    RMDIR = if exist $(subst /,\,$(1)) rd /s /q $(subst /,\,$(1))
else
    CLIENT_FULL_BIN := $(CLIENT_BIN)
    SERVER_FULL_BIN := $(SERVER_BIN)
    LDFLAGS_PLAT    :=
    CFLAGS_PLAT     :=
    MKDIR = mkdir -p $(1)
    RMDIR = rm -rf $(1)
endif

RELEASE_LDFLAGS := -s
CFLAGS_BASE     := -Wall -Wpedantic -I$(INC_DIR) -Ivendor/stk/include -Ivendor/stk/include/stk -std=c99 $(CFLAGS_PLAT)

ALL_CLIENT_SRCS := $(CLIENT_SRCS) $(ENGINE_SRCS) $(STK_SRCS)
ALL_SERVER_SRCS := $(SERVER_SRCS) $(ENGINE_SRCS) $(STK_SRCS)

CLIENT_DEBUG_OBJS   := $(ALL_CLIENT_SRCS:%.c=obj/debug/%.o)
CLIENT_RELEASE_OBJS := $(ALL_CLIENT_SRCS:%.c=obj/release/%.o)
SERVER_DEBUG_OBJS   := $(ALL_SERVER_SRCS:%.c=obj/debug/%.o)
SERVER_RELEASE_OBJS := $(ALL_SERVER_SRCS:%.c=obj/release/%.o)

.PHONY: all debug release client server clean
all: debug

debug: \
    $(BIN_DIR)/debug/$(CLIENT_FULL_BIN) \
    $(BIN_DIR)/debug/$(SERVER_FULL_BIN)

release: \
    $(BIN_DIR)/release/$(CLIENT_FULL_BIN) \
    $(BIN_DIR)/release/$(SERVER_FULL_BIN)

client: $(BIN_DIR)/debug/$(CLIENT_FULL_BIN)
server: $(BIN_DIR)/debug/$(SERVER_FULL_BIN)

# Debug Rules
$(BIN_DIR)/debug/$(CLIENT_FULL_BIN): $(CLIENT_DEBUG_OBJS)
	@$(call MKDIR,$(@D))
	$(CC) -o $@ $^ $(LDFLAGS_PLAT)

$(BIN_DIR)/debug/$(SERVER_FULL_BIN): $(SERVER_DEBUG_OBJS)
	@$(call MKDIR,$(@D))
	$(CC) -o $@ $^ $(LDFLAGS_PLAT)

# Release Rules
$(BIN_DIR)/release/$(CLIENT_FULL_BIN): $(CLIENT_RELEASE_OBJS)
	@$(call MKDIR,$(@D))
	$(CC) $(RELEASE_LDFLAGS) -o $@ $^ $(LDFLAGS_PLAT)

$(BIN_DIR)/release/$(SERVER_FULL_BIN): $(SERVER_RELEASE_OBJS)
	@$(call MKDIR,$(@D))
	$(CC) $(RELEASE_LDFLAGS) -o $@ $^ $(LDFLAGS_PLAT)

# Compile Rules
obj/debug/%.o: %.c
	@$(call MKDIR,$(@D))
	$(CC) $(CFLAGS_BASE) -g -O0 -MMD -MP -c $< -o $@

obj/release/%.o: %.c
	@$(call MKDIR,$(@D))
	$(CC) $(CFLAGS_BASE) -O2 -MMD -MP -c $< -o $@

-include $(wildcard obj/debug/*.d)
-include $(wildcard obj/release/*.d)

clean:
	@$(call RMDIR,$(OBJ_DIR))
	@$(call RMDIR,$(BIN_DIR))
