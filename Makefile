export SHELL = /bin/bash
export DIR = $(shell pwd)
export INCS_DIR = $(DIR)/includes
export SRCS_DIR = $(DIR)/src
export LIBS_DIR = $(DIR)/lib
export DEPS_DIR = $(DIR)/dep
export OBJS_DIR = $(DIR)/obj
export TARGETS_DIR = $(DIR)/bin
export TESTS_DIR = $(DIR)/test

export CXX = g++
export CXXFLAGS = -std=c++1z -m64 -Wall -O3 -g

export CP = cp -rf
export INSTALL = install
export RM = rm -rf

.PHONY: all
all: 
	@$(MAKE) --no-print-directory -C $(SRCS_DIR)

.PHONY: clean
clean:
	@$(MAKE) --no-print-directory -C $(SRCS_DIR)/ clean
	@$(RM) $(DEPS_DIR) $(OBJS_DIR) $(TARGETS_DIR) tmp
