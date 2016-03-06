
#
#	Default Makefile
#
#		2015-05, yoonji
#

CC= gcc

BIN= dev_count


CC_FLAG= -Wall -DDEBUG -g

LIB= -lpthread -lcrypto -lm

SRC= main.c device.c hash.c list.c
OBJ= $(SRC:.c=.o)

.c.o:
	@rm -f $@
	$(CC) -c -o ./$*.o ./$*.c $(CC_INCL) $(CC_FLAG)

all: $(BIN)

$(BIN): $(OBJ)
	@rm -f $@
	$(CC) -o $(BIN) $(OBJ) $(LD_INCL) $(LIB) $(CC_FLAG)
	
clean:
	rm -f $(OBJ) $(BIN)

