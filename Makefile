CXX = clang++
CXX_FLAGS = -Wall -Wextra -std=c++14 -ggdb # -fsanitize=address -fsanitize=undefined

SOURCES:=$(wildcard *.cxx)
HEADERS:=$(wildcard *.hxx)

main: $(SOURCES) $(HEADERS)
	$(CXX) $(CXX_FLAGS) -o $@ $(SOURCES)

