MAKEFLAGS += --no-print-directory
EXEC = checkers
DIR = build

.PHONY: all, check, clean

all: build
	make -C $(DIR);

$(DIR):
	mkdir $(DIR)
	cd $(DIR); cmake ..; cd ..

check: all
	clear & ./tests/controller.sh | ./$(DIR)/$(EXEC)

clean:
	rm -rf $(DIR)
