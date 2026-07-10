CC := cl
AR := lib
LD := link

OBJEXT := .obj
DEPEXT :=
STAEXT := .lib
STAPRE :=

LIB_PREFIX :=
LIB_PATH_PREFIX := /LIBPATH:
INCLUDE_PREFIX := /I

expand_libs = $(addsuffix $(STAEXT),$(1))
expand_libs_path = $(addprefix $(LIB_PATH_PREFIX),$(1))
expand_includes = $(addprefix $(INCLUDE_PREFIX),$(1))

CFLAGS := /std:c$(CVERSION) /W4
LDFLAGS :=
CPPFLAGS := $(call expand_includes,include)

ifeq ($(ENABLE_WARNINGS_AS_ERRORS),1)
	CFLAGS += /WX
endif

ifeq ($(MODE),release)
	CFLAGS += /O2 /DNDEBUG
else
	CFLAGS += /Od /Zi /DDEBUG
endif

ifeq ($(ENABLE_LTO),1)
	CFLAGS += /GL
	LDFLAGS += /LTCG
endif

ifeq ($(ENABLE_SANITIZERS),1)
	CFLAGS += /fsanitize=address
	LDFLAGS += /DEBUG
endif

CFLAGS += $(EXTRA_CFLAGS)
LDFLAGS += $(EXTRA_LDFLAGS)
CPPFLAGS += $(call expand_includes,$(EXTRA_INCLUDES))

COMPILE.c = $(CC) /nologo $(CPPFLAGS) $(CFLAGS) /c "$(1)" /Fo"$@"
LINK.exe = $(LD) /nologo $(LDFLAGS) $(1) /OUT:"$@" $(2)
LINK.static = $(AR) /nologo /OUT:"$@" $(1)