
SHELL=/bin/bash
# SUBDIRS = $(wildcard */.)
SOURCES = $(wildcard *.c) $(wildcard **/*.c)
# SRC = $(shell find src -name "*.c")
BIN = BIN



rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# How to recursively find all files with the same name in a given folder
ALL_C := $(call rwildcard,src/,*.c)

clean :
	@echo $(ALL_C)
	# @echo $(SRC)

# runval : clean testing123
#     valgrind --track-origins=yes --error-exitcode=2 --leak-check=full ./testing123
	 