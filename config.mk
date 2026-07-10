# =====================================
# config.mk template v0.6
# =====================================
# This file simplifies the use of global custom building options
#
# How this works
#
#	- TARGETS: A list with the names of all targets.
#				Each target must comply:
# 					- [target]_DIR = Source files, all subdirs will be scanned
#					- [target]_TYPE = Either: executable, static or shared
#				Optionally:
#					- [target]_LIBS = libraries to be link againts without -l flag
#					- [target]_LIBS_PATH = The path where each optional library to be link againts lives without -L flag
#
#	- CVERSION: c version number (-std=gnuX or /std:cX)
#	- EXTRA_CFLAGS: Optional compiler flags
# 	- EXTRA_LDFLAGS : Optional linker flags
#	- EXTRA_INCLUDES: Optional include directories without -I flag
#	
#	- TESTS_LIBS: Optional libraries to be link againts all tests without -l flag
#	- TESTS_LIBS_PATH: The path where each optional library to be link againts all tests lives without -L flag
#	
#	- PRE_BUILD: Command to run before target build
#	- POST_BUILD: Commant to run after target build
#
#	- ENABLE_LTO: Set to 1 to enable -flto
#	- ENABLE_SANITIZERS: Set to 1 to enable -fsanitize=address,undefined (undefined is not supported on msvc)
#	- ENABLE_WARNINGS_AS_ERRORS: Set to 1 to enable -Werror
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
TESTS_LIBS_PATH := lib

PRE_BUILD :=
POST_BUILD :=

ENABLE_LTO := 0
ENABLE_SANITIZERS := 0
ENABLE_WARNINGS_AS_ERRORS := 0