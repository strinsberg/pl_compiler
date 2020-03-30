CXX = g++
CXXFLAGS = -std=c++11 -g -Wall

SRC = src
INCLUDE = include

STATIC_ANALYSIS = cppcheck

COMPILER = compiler


.PHONY: all
all: $(COMPILER) memcheck static

# default rule for compiling .cc to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf *~ *.o $(COMPILER) pl.out pl.asm \
	docs/html docs/latex docs/*.aux docs/*.log

$(COMPILER): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(COMPILER) -I $(INCLUDE) \
	$(SRC)/*.cc

memcheck: $(COMPILER)
	valgrind --tool=memcheck --leak-check=yes $(COMPILER) test/simpleExps.pl

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
