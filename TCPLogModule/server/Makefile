
#
#	Default Makefile
#
#		2015-12, yoonzz320
#

CC= gcc

BIN= ServerTest

CC_INCL= -I./extern/include 
CC_FLAG= -Wall -DDEBUG -g

LIB= -lpthread
LD_INCL= -L./extern/lib 

SRC= main.c server.c thread.c
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

