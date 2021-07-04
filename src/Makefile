BINDIR = ../bin
CC = clang
ARGS = -g -o
CFILES = template.c object-types.c hashcodes.c base-object.c

tests: $(BINDIR)/template $(BINDIR)/baseobject $(BINDIR)/hashcodes $(BINDIR)/objecttypes
cleantests:
	rm $(BINDIR)/baseobject
	rm $(BINDIR)/hashcodes
	rm $(BINDIR)/objecttypes
	rm $(BINDIR)/template

$(BINDIR)/baseobject: $(CFILES)
	$(CC) $(ARGS) $(BINDIR)/baseobject -Wno-incompatible-pointer-types -DTEST_BASEOBJECT $(CFILES)
	$(BINDIR)/baseobject

$(BINDIR)/hashcodes: $(CFILES)
	$(CC) $(ARGS) $(BINDIR)/hashcodes -Wno-incompatible-pointer-types -DTEST_HASHES $(CFILES)
	$(BINDIR)/hashcodes

$(BINDIR)/objecttypes: $(CFILES)
	$(CC) $(ARGS) $(BINDIR)/objecttypes -Wno-incompatible-pointer-types -DTEST_OBJECT_TYPES $(CFILES)
	$(BINDIR)/objecttypes

$(BINDIR)/template: $(CFILES)
	$(CC) $(ARGS) $(BINDIR)/template -Wno-incompatible-pointer-types -DTEST_TEMPLATE $(CFILES)
	$(BINDIR)/template
