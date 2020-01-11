CXX=g++
CXXFLAGS= -std=c++11 -g

LINKFLAGS= -lgtest

SRC = src
TEST = test
INCLUDE = -I include

STATIC_ANALYSIS = cppcheck
STYLE_CHECK = cpplint.py

PROGRAM = compiler
PROGRAM_TEST = testcomp

.PHONY: all
all: $(PROGRAM_TEST) memcheck static style

# default rule for compiling .cc to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf *~ $(SRC)/*.o $(TEST_DIR)/output/*.dat \
	*.gcov *.gcda *.gcno *.orig ???*/*.orig \
	*.bak ???*/*.bak \
	???*/*~ ???*/???*/*~ $(COVERAGE_RESULTS) \
	$(PROGRAM_TEST) $(MEMCHECK_RESULTS) $(COVERAGE_DIR)  \
	docs/code/html docs/code/doxygen.log obj bin $(PROGRAM)

$(PROGRAM_TEST): $(TEST) $(SRC)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM_TEST) $(INCLUDE) \
	$(TEST)/*.cc $(SRC)/*.cc $(LINKFLAGS)

$(PROGRAM): $(SRC_DIR)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) $(INCLUDE) \
	$(SRC)/*.cc

tests: $(PROGRAM_TEST)
	$(PROGRAM_TEST)

memcheck: $(PROGRAM_TEST)
	valgrind --tool=memcheck --leak-check=yes $(PROGRAM_TEST)

static: ${SRC} ${TEST}
	${STATIC_ANALYSIS} --verbose --enable=all ${SRC} ${TEST} ${INCLUDE} --suppress=missingInclude

style: ${SRC} ${TEST} ${INCLUDE}
	${STYLE_CHECK} $(INCLUDE)/* ${SRC}/* ${TEST}/*
