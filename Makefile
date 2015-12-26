CXX = clang++
# CXX_FLAGS = -Wall -Wextra -std=c++14 -ggdb # -fsanitize=address -fsanitize=undefined
CXX_FLAGS = -Wall -Wextra -std=c++14 -O3 # -fsanitize=address -fsanitize=undefined

SOURCES:=$(wildcard *.cxx)
HEADERS:=$(wildcard *.hxx)
TESTS:=$(wildcard tests/*.in)
OUTPUTS:=$(patsubst tests/%.in,tests/%.out,$(TESTS))

tests: $(OUTPUTS)

main: $(SOURCES) $(HEADERS)
	$(CXX) $(CXX_FLAGS) -o $@ $(SOURCES)

%.out: %.in main
	./main <$< | tee $@

