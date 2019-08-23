vpath %.c = code/
vpath %.c = lib/
vpath %.so = lib/


main:libbmp.so main.c 
	$(MAKE) -C code/ main

libbmp.so:
	$(MAKE) -C lib/ libbmp.so

distclean:
	$(MAKE) -C lib/ clean
	$(MAKE) -C code/ clean

.PHONY:distclean
