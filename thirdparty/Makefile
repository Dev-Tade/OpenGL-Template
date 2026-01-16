### Thirdparty Makefile ###
# Build thirdparty srcs
# Precompile header only libraries
# Copy dll when running GLFW dynamic

include ../Config.mk

THIRDPARTY_OBJ = $(OUTPUT_DIR)/glad.o $(OUTPUT_DIR)/stb_image.o $(OUTPUT_DIR)/raymath.o

.PHONY: configure_glfw
all: configure_glfw $(THIRDPARTY_OBJ)

configure_glfw:
ifeq ($(GLFW_MODE),dynamic)
	cp "$(THIRDPARTY_LIB)/glfw3.dll" "$(BIN_DIR)/glfw.dll"
endif
	@echo " * Configuring GLFW to \"$(GLFW_MODE)\""

$(OUTPUT_DIR)/glad.o: $(THIRDPARTY_SRC)/glad.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(THIRDPARTY_INCLUDE)/

$(OUTPUT_DIR)/stb_image.o: $(THIRDPARTY_INCLUDE)/stb_image.h
	$(CC) $(CFLAGS) -x c -c $< -o $@ -DSTB_IMAGE_IMPLEMENTATION

$(OUTPUT_DIR)/raymath.o: $(THIRDPARTY_INCLUDE)/raymath.h
	$(CC) $(CFLAGS) -x c -c $< -o $@ -DRAYMATH_IMPLEMENTATION

clean:
ifeq ($(GLFW_MODE),dynamic)
	rm -rf "$(BIN_DIR)/glfw3.dll"
endif
	rm -rf $(call QUOTE_FILES,$(THIRDPARTY_OBJ))