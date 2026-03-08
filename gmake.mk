include config.mk

ifeq ($(OS),Windows_NT)
    SHELL := cmd.exe
    FULL_BIN := $(BIN_NAME).exe
    LDFLAGS_PLAT :=
    CFLAGS_PLAT :=
    MKDIR = if not exist $(subst /,\,$(1)) mkdir $(subst /,\,$(1))
    RMDIR = if exist $(subst /,\,$(1)) rd /s /q $(subst /,\,$(1))
else
    FULL_BIN := $(BIN_NAME)
    LDFLAGS_PLAT :=
    CFLAGS_PLAT :=
    MKDIR = mkdir -p $(1)
    RMDIR = rm -rf $(1)
endif

RELEASE_LDFLAGS := -s
CFLAGS_BASE := -Wall -Wpedantic -I$(INC_DIR) -std=c99 $(CFLAGS_PLAT)

.PHONY: all debug release clean

all: debug

debug: $(BIN_DIR)/debug/$(FULL_BIN)

release: $(BIN_DIR)/release/$(FULL_BIN)

# Debug Rules
$(BIN_DIR)/debug/$(FULL_BIN): $(SRCS:src/%.c=obj/debug/%.o)
	@$(call MKDIR,$(@D))
	$(CC) -o $@ $^ $(LDFLAGS_PLAT)

obj/debug/%.o: src/%.c
	@$(call MKDIR,$(@D))
	$(CC) $(CFLAGS_BASE) -g -O0 -MMD -MP -c $< -o $@

# Release Rules
$(BIN_DIR)/release/$(FULL_BIN): $(SRCS:src/%.c=obj/release/%.o)
	@$(call MKDIR,$(@D))
	$(CC) $(RELEASE_LDFLAGS) -o $@ $^ $(LDFLAGS_PLAT)

obj/release/%.o: src/%.c
	@$(call MKDIR,$(@D))
	$(CC) $(CFLAGS_BASE) -O2 -MMD -MP -c $< -o $@

-include $(wildcard obj/debug/*.d)
-include $(wildcard obj/release/*.d)

clean:
	@$(call RMDIR,$(OBJ_DIR))
	@$(call RMDIR,$(BIN_DIR))
