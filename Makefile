THIRDPARTY := -I./thirdparty/include/ -L./thirdparty/lib
LOCAL := -I./include

LINK := -lglfw3 -lopengl32 -lgdi32
GLAD := ./thirdparty/glad.c

GL_DEBUG = -DENABLE_GL_DEBUG=1

CC := gcc
CFLAGS := -Wall $(THIRDPARTY) $(LOCAL) $(GL_DEBUG)

SRC_DIR := ./src
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.c)

BUILD_DIR := ./build
OUTPUT := ./build/output

ifeq ($(OS),Windows_NT)
    RM = del /Q /F
else
    RM = rm -f
endif

ifeq ($(OS),Windows_NT)
	RMALL := .\\build\\*
else
	RMALL := ./build/*
endif

all: build

build: $(OUTPUT)

$(OUTPUT): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(OUTPUT) $^ $(GLAD) $(LINK)

clean:
	$(RM) $(RMALL)

run: $(OUTPUT)
	$(OUTPUT)