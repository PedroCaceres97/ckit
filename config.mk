# =====================================
# config.mk template v0.6
# =====================================
#
# This file contains the project-specific build configuration.
# Most projects only need to edit this file; the build logic is
# implemented by the Makefile and the modules under makefiles/.
#
# Configuration:
#
#   TARGETS
#       Space-separated list of build targets.
#
#   For each target listed in TARGETS:
#       [target]_DIR
#           Directory containing the target source files.
#           All subdirectories are scanned recursively.
#
#       [target]_TYPE
#           Target type: executable, static or shared.
#
#       [target]_LIBS
#           Optional libraries to link against.
#           Specify library names only (without the platform prefix/suffix).
#
#       [target]_LIBS_PATH
#           Optional library search directories.
#           Specify paths only (without -L or /LIBPATH:).
#
#   CVERSION
#       C language version passed to the selected toolchain.
#
#   EXTRA_CFLAGS
#       Additional compiler flags.
#
#   EXTRA_LDFLAGS
#       Additional linker flags.
#
#   EXTRA_INCLUDES
#       Additional include directories.
#       Specify paths only (without -I or /I).
#
#   TESTS_LIBS
#       Libraries linked against every test executable (without the platform prefix/suffix).
#
#   TESTS_LIBS_PATH
#       Library search directories used by every test executable (without -L or /LIBPATH:).
#
#   PRE_BUILD
#       Command executed before building each target.
#
#   POST_BUILD
#       Command executed after building each target.
#
#   ENABLE_LTO
#       Enables Link Time Optimization when supported by the toolchain.
#
#   ENABLE_SANITIZERS
#       Enables AddressSanitizer and UndefinedBehaviorSanitizer when
#       supported by the selected toolchain. Forces debug mode.
#
#   ENABLE_WARNINGS_AS_ERRORS
#       Treats all compiler warnings as errors.
#
#   DISABLE_DEPRACATED_WARNINGS
#       Disables depracated functions warnings.
#
# =====================================
# EDIT AT EASE
# =====================================

# =====================================
# Targets
# =====================================

DEFAULT := $(notdir $(CURDIR))
$(DEFAULT)_DIR 			:= src
$(DEFAULT)_TYPE			:= static
$(DEFAULT)_LIBS			:= 
$(DEFAULT)_LIBS_PATH	:= 

TARGETS := $(DEFAULT)

# =====================================
# Configuration
# =====================================

CVERSION := 23
EXTRA_CFLAGS :=
EXTRA_LDFLAGS :=
EXTRA_INCLUDES :=

TESTS_LIBS := $(DEFAULT)
TESTS_LIBS_PATH := lib debug/lib

PRE_BUILD :=
POST_BUILD :=

ENABLE_LTO := 0
ENABLE_SANITIZERS := 0
ENABLE_WARNINGS_AS_ERRORS := 0
DISABLE_DEPRACATED_WARNINGS := 1