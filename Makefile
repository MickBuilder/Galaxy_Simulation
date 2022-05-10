###### VARIABLES #######
ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	RM = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	RM = rm -f
	MKDIR = mkdir -p
  endif
	LDFLAGS = -L lib -L final_product -lMLV-0 -lSDL -lmingw32 -lSDLmain ./lib/libmingwex.a -lm
	EXTENSION=exe
else
	LDFLAGS = `pkg-config --cflags MLV` `pkg-config --libs-only-other --libs-only-L MLV` `pkg-config --libs-only-l MLV`
	RM = rm -f
	MKDIR = mkdir -p
	EXTENSION=out
endif

CC = gcc
CFLAGS 	= -Wall -pedantic -I include
RM = rm -f

SRC 			= ./src
TESTDIR 		= ./tests
OBJ 			= ./obj
BIN 			= ./bin

sources 		= $(wildcard $(SRC)/*.c)
test_sources 	= $(wildcard $(TESTDIR)/*.c)
objects 		= $(sources:$(SRC)/%.c=$(OBJ)/%.o)
test_objects 	= $(test_sources:$(TESTDIR)/%.c=$(OBJ)/%.o)

MAIN = $(BIN)/galaxy.$(EXTENSION)
TEST = test.$(EXTENSION)

###### RULES #######

all: $(MAIN) #$(TEST)

$(MAIN): $(objects)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
 
$(TEST): $(test_objects) $(OBJ)/toto.o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ):
	mkdir $@

$(objects): $(OBJ)/%.o : $(SRC)/%.c $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(test_objects): $(OBJ)/%.o : $(TESTDIR)/%.c $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@


run: $(MAIN)
	./$(MAIN)

test: $(TEST)
	./$(TEST)

clean:
	$(RM) -r $(OBJ)
	$(RM) $(MAIN)
	$(RM) $(TEST)

.PHONY: all clean run test