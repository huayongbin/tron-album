SRC = $(wildcard *.c)

OBJ = $(SRC:%.c=%.o)
BIN = $(SRC:%.c=%)

CC=arm-linux-gcc
CPPFLAGS += -I ../include
CPPFLAGS += -I /home/gec/AAA/include

LDFLAGS += -L /home/gec/AAA/lib
LDFLAGS += -ljpeg


libbmp.so:libjpg.so
	$(CC) -shared -fPIC -o libbmp.so bmp.o $(CPPFLAGS)
	
libjpg.so:$(OBJ)
	$(CC) -shared -fPIC -o libjpg.so jpg.o $(CPPFLAGS) $(LDFLAGS)

$(OBJ):%.o:%.c
	$(CC) $^ -o $@ -c -fPIC $(CPPFLAGS)

clean:
	-@$(RM) $(OBJ)
	-@$(RM) libbmp.so
	-@$(RM) libjpg.so

.PHONY:clean
