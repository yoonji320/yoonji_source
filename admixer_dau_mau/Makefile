
#
#	Default Makefile
#
#		2015-12, yoonzz320
#

CC= gcc

BIN= admixer_dau

MYSQLDIR= /usr/local/mysql

CC_INCL= -I./extern/include -I$(MYSQLDIR)/include/mysql
CC_FLAG= -Wall -DDEBUG -g

LIB= $(MYSQLDIR)/lib/mysql/libmysqlclient.a -lm -lz
LD_INCL= -L./extern/lib -L$(MYSQLDIR)/lib/mysql

SRC= main.c admixer_dau.c hash.c dev.c app.c net.c report.c config.c dbman.c
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

