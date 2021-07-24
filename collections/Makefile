#### new make
# CC = clang
CC = gcc
ODIR = obj
SDIR = src
BDIR = bin
HDIR = header
TDIR = test
LDIR = lib
TBDIR = testbin
SRCTEST = $(shell ls $(TDIR))
SRC = $(shell ls $(SDIR))
TESTMAIN = $(SRCTEST:%.c=$(TBDIR)/%)
OBJFILES = $(SRC:%.c=$(ODIR)/%.o)
STATICLIB = $(LDIR)/libcrlist.so

ARGS = -Wno-incompatible-pointer-types -Wno-incompatible-function-pointer-types -fPIE -march=native -g -I$(HDIR)
# ARGS = -fPIE -march=native -g -I$(HDIR)
# ARGS = -fPIE -march=native -g -I$(HDIR)

$(TBDIR)/%: $(TDIR)/%.c
	@echo "\033[0;32mCompile test $@\033[0m"
	# $(CC) $(ARGS) $(STATICLIB) -o  $@ $< $(CFLAGS)
	$(CC) $(ARGS) $(OBJFILES) lib/cmocka.c.o  -o $@ $< $(CFLAGS)
	@echo "\033[0;32mRun test $@\033[0m"
	time $@

$(ODIR)/%.o: $(SDIR)/%.c
	@echo "\033[0;32mCompile objects $@\033[0m"
	$(CC) $(ARGS) -c -o $@ $< $(CFLAGS)

.PHONY: clean

compile: $(OBJFILES) $(STATICLIB) $(TESTMAIN)

clean:
	@echo "\033[0;32mDeleting build artifacts..\033[0m"
	rm -f $(ODIR)/*.o $(BDIR)/* $(TBDIR)/* $(STATICLIB)

cleantest:
	@echo "\033[0;32mDeleting tests binaries..\033[0m"
	rm -f $(TBDIR)/*

$(STATICLIB): 
	@echo "\033[0;32mCOMPILING SOURCE $< INTO Dynamic Lib $@\033[0m"
	@$(CC)  $(ARGS) -shared -fpic $(SDIR)/*.c $< -o $@	

runtests: cleantest $(TESTMAIN)