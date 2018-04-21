# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of GNU's libstdc++.
# -g is for debugging.
CPPFLAGS =  -std=c++11 -I.
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += -std=c++11
CXXFLAGS += -g
CXXFLAGS += $(DEPFLAGS)
LDFLAGS =   -g -L.
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++

# Libraries
LDLIBS = -lclientserver

# Targets
TARGETS = libclientserver.a client server
all: $(TARGETS)

# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

libclientserver.a: connection.o server.o
	ar rv libclientserver.a  connection.o server.o
	ranlib libclientserver.a

client: client_main.o messagehandler.o
server: server_main.o messagehandler.o memdb.o diskdb.o diskutils.o

bin:
	mkdir bin
servercpy:
	cp ./server ./bin
clientcpy:
	cp ./client ./bin

install: bin servercpy clientcpy


# Phony targets
.PHONY: all clean distclean

# Standard clean
clean:
	rm -f *.o $(TARGETS)

distclean: clean
	-rm *.d libclientserver.a


# Include the *.d files
SRC = $(wildcard *.cc)
-include $(SRC:.cc=.d)

