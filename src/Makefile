### User Makefile ###
# Build user source code

include ../Config.mk

SRC := $(wildcard *.c)
OBJ := $(SRC:%.c=$(OUTPUT_DIR)/%.o)
THIRDPARTY_OBJ = $(filter-out $(OBJ),$(wildcard $(OUTPUT_DIR)/*.o))

BIN := $(BIN_DIR)/$(OUTPUT_EXEC)

INCLUDES := -I$(THIRDPARTY_INCLUDE)/ -I$(USER_INCLUDE)/
LIBS := -L$(THIRDPARTY_LIB)

ifeq ($(GLFW_MODE),static)
	LIBS += -lglfw3 -lopengl32 -lgdi32
else ifeq ($(GLFW_MODE),dynamic)
	LIBS += -DGLFW_DLL -lglfw3dll
endif


.PHONY: all clean

all: $(BIN)

$(OUTPUT_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(BIN): $(OBJ) $(THIRDPARTY_OBJ)
	$(CC) $^ -o $@ $(LIBS) $(INCLUDES)

clean:
	rm -rf $(call QUOTE_FILES,$(OBJ) $(BIN))