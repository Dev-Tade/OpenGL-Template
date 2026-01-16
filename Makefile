### Root Makefile ###

include Config.mk

# Base Build Options

# Always run thirdparty (internally skips already built dependencies)
.PHONY: thirdparty

all: check thirdparty user

# Check Source Tree
check:
	@echo "-- Checking project source tree..."
	@test -d $(THIRDPARTY_DIR)/ || (echo " * ERROR: THIRDPARTY_DIR: $(THIRDPARTY_DIR)/ is missing" && exit 1)
	@test -d $(USER_INCLUDE)/ 	|| (echo " * ERROR: USER_INCLUDE: $(USER_INCLUDE)/ is missing" && exit 1)
	@test -d $(USER_SRC)/ 			|| (echo " * ERROR: USER_SRC: $(USER_SRC)/ is missing" && exit 1)
	@test -d $(OUTPUT_DIR)/ 		|| (mkdir -p $(OUTPUT_DIR) && echo " * Generated missing OUTPUT_DIR: $(OUTPUT_DIR)")
	@test -d $(BIN_DIR)/ 				|| (mkdir -p $(BIN_DIR) && echo " * Generated missing BIN_DIR: $(BIN_DIR)")
	@echo "-- Project source tree is correct"

clean:
	@$(MAKE) -C $(USER_SRC) clean --no-print-directory
ifeq ($(CLEAN_THIRDPARTY),yes)
	@$(MAKE) -C $(THIRDPARTY_DIR) clean --no-print-directory
endif

run:
	$(BIN_DIR)/$(OUTPUT_EXEC_NAME)

thirdparty:
	@echo "-- Building thirdparty dependencies..."
	@$(MAKE) -C $(THIRDPARTY_DIR) --no-print-directory

user:
	@echo "-- Building user code"
	@$(MAKE) -C $(USER_SRC) --no-print-directory