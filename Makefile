CXX = g++
CXXFLAGS = -std=c++11 -g -Wall

LINKFLAGS = -lgtest

SRC = src
TEST = test
INCLUDE = include

STATIC_ANALYSIS = cppcheck

PROGRAM = compiler
PROGRAM_TEST = testcomp

.PHONY: all
all: $(PROGRAM_TEST) memcheck static

# default rule for compiling .cc to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf *~ *.o $(PROGRAM_TEST) $(PROGRAM)

$(PROGRAM_TEST): $(TEST) $(SRC)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM_TEST) -I $(INCLUDE) \
	$(TEST)/*.cc $(SRC)/[^p]*.cc $(LINKFLAGS)

$(PROGRAM): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) -I $(INCLUDE) \
	$(SRC)/*.cc

tests: $(PROGRAM_TEST)
	$(PROGRAM_TEST)

memcheck: $(PROGRAM_TEST)
	valgrind --tool=memcheck --leak-check=yes $(PROGRAM_TEST)

static: $(SRC) $(TEST)
	$(STATIC_ANALYSIS) --verbose --enable=all $(SRC) $(TEST) $(INCLUDE) --suppress=missingInclude
