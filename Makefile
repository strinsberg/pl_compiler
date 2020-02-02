CXX = g++
CXXFLAGS = -std=c++11 -g -Wall

SRC = src
INCLUDE = include

STATIC_ANALYSIS = cppcheck

PROGRAM = compiler

.PHONY: all
all: $(PROGRAM) memcheck static

# default rule for compiling .cc to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf *~ *.o $(PROGRAM) pl.out \
	docs/html docs/latex docs/*.aux docs/*.log

$(PROGRAM): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(PROGRAM) -I $(INCLUDE) \
	$(SRC)/*.cc

memcheck: $(PROGRAM)
	valgrind --tool=memcheck --leak-check=yes $(PROGRAM) test/simpleExps.pl

static: $(SRC)
	$(STATIC_ANALYSIS) --verbose --enable=all $(SRC) $(TEST) $(INCLUDE) --suppress=missingInclude

.PHONY: docs
docs: $(INCLUDE)
	doxygen docs/doxyfile
	cd docs/latex; make
	cd docs; pdflatex README.tex; pdflatex TechnicalManual.tex
	mv docs/latex/refman.pdf ReferenceDoc.pdf
	mv docs/README.pdf README.pdf
	mv docs/TechnicalManual.pdf TechnicalManual.pdf
