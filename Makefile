# =====================================
# Makefile template v0.6
# =====================================
# 
# This project provides a reusable Makefile template for C projects.
# The template supports multiple build targets and multiple compiler toolchains.
# Every .c source file located inside the tests/ directory is treated as an independent executable.
# Tests are discovered automatically without requiring manual registration. 
# Additional libraries and include directories may be configured globally 
# or individually through variables in config.mk.
#
# Most project-specific settings are placed in config.mk, while the build logic 
# is split into small modules under the makefiles/ directory.
#
# Project Structure:
# .
# ├── bin/
# ├── debug/
# ├── include/
# ├── lib/
# ├── makefiles/
# │   ├── architecture.mk
# │   ├── target-generator.mk
# │   ├── test-generator.mk
# │   ├── gcc.mk
# │   ├── clang-gnu.mk
# │   ├── clang-msvc.mk
# │   └── msvc.mk
# ├── obj/
# ├── src/
# ├── tests/
# ├── config.mk
# └── Makefile
#
# Build artifacts are generated automatically:
# 	bin/ – Executables.
# 	lib/ – Static libraries.
# 	obj/ – Object files and dependency files.
# 	debug/ – Debug equivalents of the previous directories.
#
# Phonies:
# 	all (default): Builds every target listed in TARGETS.
# 	clean: removes all generated files.
# 	debug: same as "make all MODE=debug"
# 	tests: builds every test
#	tests-debug: same as "make tests MODE=debug"
#
# Toolchains:
# The compiler can be selected when invoking Make
# 	make TOOLCHAIN=gcc 
# 	make TOOLCHAIN=msvc
#	make TOOLCHAIN=clang (MSYS2 version of windows)
#	make TOOLCHAIN=clang-msvc (Official version of windows)
#
# Each toolchain module defines compiler commands, 
# compiler and linker flags, library naming conventions, 
# and file extensions appropriate for the selected environment.
#
# =====================================
# EDIT WITH HIGH PRECAUTION.
# =====================================

include config.mk
ifeq ($(ENABLE_LTO),1)
    MODE := debug
endif 
ifeq ($(ENABLE_SANITIZERS),1)
    MODE := debug
endif

MODE ?= release
TOOLCHAIN ?= clang-msvc
include makefiles/$(TOOLCHAIN).mk
include makefiles/architecture.mk
include makefiles/test-generator.mk
include makefiles/target-generator.mk

rwildcard = $(filter $(subst *,%,$2),$(wildcard $1/$2)) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))

TESTS_LDFLAGS := $(call expand_libs_path,$(TESTS_LIBS_PATH)) $(call expand_libs,$(TESTS_LIBS))
TESTS_SRC := $(call rwildcard,$(TESTS_FOLDER),*.c)
TESTS := $(patsubst $(TESTS_FOLDER)/%.c,%,$(TESTS_SRC))

.PHONY: all clean debug tests tests-debug

all: $(TARGETS)

clean:
	@$(call RMDIR,$(OBJ_FOLDER))
	@$(call RMDIR,$(BIN_FOLDER))
	@$(call RMDIR,$(LIB_FOLDER))
	@$(call RMDIR,$(DEBUG_FOLDER))

debug:
	@$(MAKE) MODE=debug all

tests: $(addsuffix $(EXEEXT), $(addprefix $(BIN_PATH)/$(TESTS_FOLDER)/,$(TESTS)))

tests-debug:
	@$(MAKE) MODE=debug tests

$(foreach test,$(TESTS),$(eval $(call DEFINE_TEST,$(test))))
$(foreach target,$(TARGETS),$(eval $(call DEFINE_TARGET,$(target))))