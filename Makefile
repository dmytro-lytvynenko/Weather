# A sample Makefile for building Google Test and using it in user
# tests.  Please tweak it to suit your environment and project.  You
# may want to move it to your project's root directory.
#
# SYNOPSIS:
#
#   make [all]  - makes everything.
#   make TARGET - makes the given target.
#   make clean  - removes all files generated by make.

# Please tweak the following variable definitions as needed by your
# project, except GTEST_HEADERS, which you can use in your own targets
# but shouldn't modify.

# Points to the root of Google Test, relative to where this file is.
# Remember to tweak this if you move this file.
ifndef GTEST_DIR
GTEST_DIR = ../googletest/googletest
endif

# Flags passed to the preprocessor.
# Set Google Test's header directory as a system directory, such that
# the compiler doesn't generate warnings in Google Test headers.
CPPFLAGS += -isystem $(GTEST_DIR)/include

# Flags passed to the C++ compiler.
CXXFLAGS += -g -std=c++11 -Wall -Wextra -pthread -pedantic -Werror

# Flags passed to the C++ compiler for compiling coder_gTest.o
CXXFLAGS_FOR_TESTS += -g -std=c++11 -Wall -Wextra -pthread -pedantic

# All tests produced by this Makefile.  Remember to add new tests you
# created to the list.
TESTS = coder_gTest

SOURCE_DIR = project
LIB_DIR = project
TEST_DIR = test

# Setup static library name.
KERNEL_NAME = $(shell uname -s)
MACHINE_NAME = $(shell uname -m)

ifeq ($(KERNEL_NAME),Linux)
ifeq ($(MACHINE_NAME),i386)
LIB_ENCODE=encode_linux_i386
endif
ifeq ($(MACHINE_NAME),x86_64)
LIB_ENCODE=encode_linux_x86_64
endif
endif

ifneq (,$(findstring CYGWIN,$(KERNEL_NAME)))
ifeq ($(MACHINE_NAME),x86_64)
LIB_ENCODE=encode_cygwin_x86_64
endif
ifeq ($(MACHINE_NAME),i686)
LIB_ENCODE=encode_cygwin_i686
endif
endif

ifneq (,$(findstring MINGW,$(KERNEL_NAME)))
endif

ifneq (,$(findstring Darwin,$(KERNEL_NAME)))
ifeq ($(MACHINE_NAME),x86_64)
LIB_ENCODE=encode_mac_x86_64
endif
endif

ifeq "$(shell echo $(LIB_ENCODE))" ""
$(error Static library for $(KERNEL_NAME) $(MACHINE_NAME) platform is not shipped.)
endif

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

# House-keeping build targets.
all : $(TESTS)

clean :
	rm -fr $(TESTS) gtest.a gtest_main.a *.o   

cppcheck :
	cppcheck -q --enable=warning,style,performance,portability $(SOURCE_DIR)

# Builds gtest.a and gtest_main.a.

# Usually you shouldn't tweak such internal variables, indicated by a
# trailing _.
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's
# implementation details, the dependencies specified below are
# conservative and not optimized.  This is fine as Google Test
# compiles fast and for ordinary users its source rarely changes.
gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS_FOR_TESTS) -c \
            $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# Builds a sample test.  A test should link with either gtest.a or
# gtest_main.a, depending on whether it defines its own main()
# function.
coder.o : $(SOURCE_DIR)/coder.cpp $(SOURCE_DIR)/coder.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SOURCE_DIR)/coder.cpp

coder_gTest.o : $(TEST_DIR)/coder_gTest.cpp $(SOURCE_DIR)/coder.h $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) -I$(SOURCE_DIR) $(CXXFLAGS_FOR_TESTS) -c $(TEST_DIR)/coder_gTest.cpp

coder_gTest : coder.o coder_gTest.o gtest_main.a
	@echo "Building $@ for $(KERNEL_NAME) $(MACHINE_NAME)"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@ -L. -L$(LIB_DIR) -l$(LIB_ENCODE)
	./$(TESTS)
