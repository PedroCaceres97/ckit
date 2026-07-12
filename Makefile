# =====================================
# Makefile template v0.6
# =====================================
#
# Reusable Makefile template for C projects.
#
# Supports multiple build targets, compiler toolchains, and automatic
# test discovery. Every .c source file inside the `tests/` directory is
# compiled as an independent executable.
#
# This template separates project configuration from build logic. 
# Most projects only require editing `config.mk`, while the build rules
# remain in the `Makefile` and the modules under `makefiles/`.
#
# Project Layout:
# .
# ├── bin/
# │   └── tests/
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
# Generated Directories:
#   `bin/`         Executable targets.
#   `bin/tests/`   Test executables.
#   `lib/`         Static libraries.
#   `obj/`         Object and dependency files.
#   `debug/`       Debug build output.
#
# Phony Targets:
#   all          Build every target listed in TARGETS (default).
#   clean        Remove all generated files.
#   debug        Equivalent to "make MODE=debug".
#   tests        Build every test under tests/.
#   tests-debug  Equivalent to "make MODE=debug tests".
#
# Toolchains:
#   make TOOLCHAIN=gcc
#   make TOOLCHAIN=msvc
#   make TOOLCHAIN=clang        (GNU-style Clang)
#   make TOOLCHAIN=clang-msvc   (MSVC-compatible Clang)
#
# Each toolchain module defines the compiler and archiver commands,
# compilation and linking flags, library naming conventions, and file
# extensions required by its target environment.
#
# gcc
#   Uses the GNU toolchain (gcc, gcc-ar). Produces ELF binaries on Unix-like
#   systems and PE binaries through MinGW on Windows. Generates .o objects,
#   .a static libraries, and dependency files automatically.
#
# msvc
#   Uses Microsoft's native compiler and linker (cl, link, lib). Produces
#   .obj object files and .lib static libraries using the Microsoft ABI and
#   command-line conventions. Intended for Visual Studio or Build Tools.
#
# clang
#   Uses Clang with the GNU driver (clang, llvm-ar). Compatible with GCC-style
#   command-line options and intended for Unix-like environments or Windows
#   through MinGW/MSYS2. Produces the same file formats as gcc.
#
# clang-msvc
#   Uses the official LLVM/Clang compiler targeting the Microsoft ABI.
#   Generates MSVC-compatible .lib libraries while keeping the Clang/GNU-style
#   command-line interface. Intended for the official LLVM distribution on
#   Windows and interoperable with MSVC libraries.
#
# =====================================
# EDIT WITH HIGH PRECAUTION.
# =====================================

include config.mk
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