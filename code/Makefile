SRC := $(wildcard *.c)
SRC := $(filter-out main.c,$(SRC))

OBJ = $(SRC:%.c=%.o)

CC=arm-linux-gcc

CPPFLAGS += -I ../include
CPPFLAGS += -I /home/gec/AAA/include

LDFLAGS += -L /home/gec/AAA/lib
LDFLAGS += -ljpeg

main:$(OBJ)
	$(CC) $(OBJ) main.c -o main $(CPPFLAGS) $(LDFLAGS) -ldl
	@mv main ../
	
$(OBJ):%.o:%.c
	$(CC) $^ -o $@ -c -fPIC $(CPPFLAGS)

clean:
	-@$(RM) $(OBJ)
	-@$(RM) ../main
	
.PHONY: clean
