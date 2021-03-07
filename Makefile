SOURCE_DIR=./src

all: execute

execute: compile
	$(SOURCE_DIR)/combat

compile: 
	make -C $(SOURCE_DIR)