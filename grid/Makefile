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
export CXXFLAGS = -std=c++11 -m64 -Wall -g -O3 -fopenmp
export INCS = -I$(INCS_DIR) -I/usr/include -I/usr/local/include
export LIBS = -L/usr/local/lib/fltk1.3 -lfltk -lfltk_gl -lfltk_images -lfltk_forms -lXext -lX11 -lGL -lGLU -lm
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
