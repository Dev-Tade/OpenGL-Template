### Config Makefile ###
# Defines user configs
# Defines directory paths
# Setups toolchain configuration

# System 
ifeq ($(OS),Windows_NT)
EXEC_EXT = .exe
endif

QUOTE_FILES = $(foreach file,$1,"$(file)")

# User configs
OUTPUT_EXEC_NAME ?= app
GLFW_MODE ?= static

# Directory paths
ROOT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

THIRDPARTY_DIR := $(ROOT_DIR)/thirdparty
THIRDPARTY_INCLUDE := $(THIRDPARTY_DIR)/include
THIRDPARTY_SRC := $(THIRDPARTY_DIR)/src
THIRDPARTY_LIB := $(THIRDPARTY_DIR)/lib

OUTPUT_DIR := $(ROOT_DIR)/build
BIN_DIR := $(OUTPUT_DIR)/bin

USER_INCLUDE := $(ROOT_DIR)/include
USER_SRC := $(ROOT_DIR)/src

OUTPUT_EXEC := $(OUTPUT_EXEC_NAME)$(EXEC_EXT)

# Setup toolchain
TOOLCHAIN := 

CC := $(TOOLCHAIN)gcc
CFLAGS := -Wall -Werror