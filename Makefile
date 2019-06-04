NAME=libbitarray
VERSION=1.0
LIBDIR=/usr/lib
INCDIR=/usr/include
LIBNAME=$(NAME)-$(VERSION).so
SYMNAME=$(NAME).so
OBJ=$(LIBNAME).o
CC=gcc
CFLAGS=-Wall -Werror -fpic -O3
SRC=bitarray.c
INC=bitarray.h

$(LIBNAME): $(SRC)
	$(CC) -o $(OBJ) -c $(CFLAGS) $<
	$(CC) -shared -o $(LIBNAME) $(OBJ)
	strip $(LIBNAME)

install:
	install $(LIBNAME) $(LIBDIR)
	cd $(LIBDIR); ln -s $(LIBNAME) $(SYMNAME)
	install $(INC) $(INCDIR)

uninstall:
	rm -vf $(LIBDIR)/$(LIBNAME)
	rm -vf $(LIBDIR)/$(SYMNAME)
	rm -vf $(INCDIR)/$(INC)

clean:
	rm -fv $(LIBNAME) $(OBJ)
