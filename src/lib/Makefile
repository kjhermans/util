CFILES:=$(shell ls *.c | sort)
OBJECTS:=$(shell ls *.c | sort | sed -e 's/\.c$$/.o/')
CC=$(ARCH)gcc
AR=$(ARCH)ar
CFLAGS=-g -O2 -Wall -Wextra -fPIC \
  -I../include -I../preproc -I../json/include $(DEBUG)
TARGET=libutil.a

all: fnchdr $(TARGET)

preproc: fnchdr

fnchdr: ../include/util/util_functions.h

../include/util/util_functions.h: $(CFILES)
	../../bin/genfnchdr \
	  -i UTIL \
	  -A '#include <btree/td.h>' \
	  -x ./table_t.c \
	  -x ./db_t.c \
	  -x ./xtransport.c \
	    > ../include/util/util_functions.h

$(TARGET): $(OBJECTS)
	$(AR) -rcs $(TARGET) $(OBJECTS)

superclean: clean
	rm -f ../include/util/util_functions.h

clean:
	rm -f $(OBJECTS) $(TARGET)
