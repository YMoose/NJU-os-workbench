NAME := kernel
SRCS := kernel.c
GRAPH_PATH ?= /home/temp/cover.bmp
GRAPH_DATA_H := graph_data.h
export MODULE  := L0
export AM_HOME := $(PWD)/../abstract-machine
ifeq ($(ARCH),)
export ARCH := x86_64-qemu
endif

include $(AM_HOME)/Makefile
include ../Makefile.lab
image: git

graph_data: $(GRAPH_PATH)
	@echo "#ifndef GRAPH_DATA_H" > $(GRAPH_DATA_H)
	@echo "#define GRAPH_DATA_H" >> $(GRAPH_DATA_H)
	@xxd -i $(GRAPH_PATH) >> $(GRAPH_DATA_H)
	@echo "#endif /* GRAPH_DATA_H */" >> $(GRAPH_DATA_H)

.PHONY: clean_graph_data
clean_graph_data:
	@rm -f $(GRAPH_DATA_H)