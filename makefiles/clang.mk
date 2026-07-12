# =====================================
# Makefile template v0.6
# =====================================
# EDIT WITH HIGH PRECAUTION.
# =====================================

CC := clang
AR := llvm-ar

OBJEXT := .o
DEPEXT := .d
STAEXT := .a
STAPRE := lib

LIB_PREFIX := -l
LIB_PATH_PREFIX := -L
INCLUDE_PREFIX := -I

expand_libs = $(addprefix $(LIB_PREFIX),$(1))
expand_libs_path = $(addprefix $(LIB_PATH_PREFIX),$(1))
expand_includes = $(addprefix $(INCLUDE_PREFIX),$(1))

CFLAGS := -std=gnu$(CVERSION) -MMD -MP -Wall -Wextra
LDFLAGS :=
CPPFLAGS := $(call expand_includes,include)

ifeq ($(ENABLE_WARNINGS_AS_ERRORS),1)
	CFLAGS += -Werror
endif

ifeq ($(MODE),release)
	CFLAGS += -O2 -DNDEBUG
else
	CFLAGS += -O0 -g -DDEBUG
endif

ifeq ($(ENABLE_LTO),1)
	CFLAGS += -flto
	LDFLAGS += -flto
endif

ifeq ($(ENABLE_SANITIZERS),1)
	CFLAGS += -fsanitize=address,undefined
	LDFLAGS += -fsanitize=address,undefined
endif

ifeq ($(DISABLE_DEPRACATED_WARNINGS),1)
	CFLAGS += -Wno-deprecated-declarations
endif

CFLAGS += $(EXTRA_CFLAGS)
LDFLAGS += $(EXTRA_LDFLAGS)
CPPFLAGS += $(call expand_includes,$(EXTRA_INCLUDES))

COMPILE.c = $(CC) $(CPPFLAGS) $(CFLAGS) -c $(1) -o $@
LINK.exe = $(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(1) -o $@ $(2)
LINK.static = $(AR) rcs $@ $(1)