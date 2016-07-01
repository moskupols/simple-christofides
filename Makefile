CXX ?= clang++
CXXFLAGS ?= -O3
CXXFLAGS += -Wall -Wextra -std=c++14

SOURCES:=$(wildcard *.cxx)
HEADERS:=$(wildcard *.hxx)
TESTS:=$(wildcard tests/*.in)
OUTPUTS:=$(patsubst tests/%.in,tests/%.out,$(TESTS))

tests: $(OUTPUTS)

main: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(SOURCES)

%.out: %.in main
	./main <$< | tee $@

