CLIBS = ../clibs
CXX = clang
CXX_FLAGS = -Wall -Wextra -ggdb -pedantic -I$(CLIBS)/raylib/include/ -Wl,-rpath=$(CLIBS)/raylib/lib/ -L$(CLIBS)/raylib/lib/ -lraylib -lm

out: main.c
	$(CXX) $(CXX_FLAGS) -o out main.c

clean:
	rm -rf out

