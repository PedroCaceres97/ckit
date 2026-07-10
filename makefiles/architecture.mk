SRC_FOLDER := src
OBJ_FOLDER := obj
BIN_FOLDER := bin
LIB_FOLDER := lib
TESTS_FOLDER := tests
DEBUG_FOLDER := debug

ifeq ($(MODE),release)
	LIB_PATH := $(LIB_FOLDER)
	BIN_PATH := $(BIN_FOLDER)
	OBJ_PATH := $(OBJ_FOLDER)
else
	LIB_PATH := $(DEBUG_FOLDER)/$(LIB_FOLDER)
	BIN_PATH := $(DEBUG_FOLDER)/$(BIN_FOLDER)
	OBJ_PATH := $(DEBUG_FOLDER)/$(OBJ_FOLDER)
endif

ifeq ($(OS),Windows_NT)
    MKDIR = mkdir "$(1)" 2>nul || exit 0
    RMDIR = if exist "$(1)" rmdir /S /Q "$(1)"
    EXEEXT := .exe
    ECHO_BLANK = echo.
else
    MKDIR = mkdir -p "$(1)"
    RMDIR = rm -rf "$(1)"
    EXEEXT :=
    ECHO_BLANK = printf "\n"
endif