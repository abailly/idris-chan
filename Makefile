RANLIB      ?=ranlib
AR          ?=ar

MACHINE         := $(shell $(CC) -dumpmachine)
ifneq (, $(findstring darwin, $(MACHINE)))
	OS      :=darwin
else ifneq (, $(findstring cygwin, $(MACHINE)))
	OS      :=windows
else ifneq (, $(findstring mingw, $(MACHINE)))
	OS      :=windows
else ifneq (, $(findstring windows, $(MACHINE)))
	OS      :=windows
else
	OS      :=unix
endif

SHLIB_SUFFIX    :=.so

LIBNAME=idris_queue
OBJS = $(LIBNAME).o
HDRS = $(LIBNAME).h
CFLAGS := $(CFLAGS)
IDRIS_SRCS =

ifneq ($(OS), windows)
	CFLAGS += -fPIC
endif

DYLIBTARGET = $(LIBNAME)$(SHLIB_SUFFIX)
LIBTARGET = $(LIBNAME).a
TARGET=`${IDRIS2} --libdir`

build: $(DYLIBTARGET) $(IDRIS_SRCS)
	@if [ -z "${IDRIS2}" ]; then echo 'variable $$IDRIS2 is not set, aborting'; exit 1; fi
	${IDRIS2} --build idris-chan.ipkg

$(DYLIBTARGET) : $(OBJS)
	$(CC) -o $(DYLIBTARGET) $(LIBFLAGS) -shared $(OBJS)

install:
	@if [ -z "${IDRIS2}" ]; then echo 'variable $$IDRIS2 is not set, aborting'; exit 1; fi
	${IDRIS2} --install idris-chan.ipkg
	@if ! [ -d $(TARGET)/idris-chan/lib ]; then mkdir $(TARGET)/idris-chan/lib; fi
	install $(DYLIBTARGET) $(HDRS) $(TARGET)/idris-chan/lib

clean :
	rm -rf $(OBJS) $(LIBTARGET) $(DYLIBTARGET) build

test_c: test.c
	cp $(DYLIBTARGET) lib$(DYLIBTARGET) # to make C linker happy
	$(CC) -o idris-chan-tests -L. -I. test.c -l$(LIBNAME)
	LD_LIBRARY_PATH=. ./idris-chan-tests

test: build test_c

$(OBJS): $(HDRS)

all: $(DYLIBTARGET) $(LIBTARGET)
	${IDRIS2} --build idris-chan.ipkg

.PHONY: install clean
