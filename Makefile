CXX = g++
CXXFLAGS = -std=c++11 -g -Wall

SRC = src
INCLUDE = include
DRIVER = drivers

COMPILER = compiler
INTERPRET = interpret

.PHONY: all
all: $(COMPILER) $(INTERPRET)

# default rule for compiling .cc to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf *~ *.o $(COMPILER) $(INTERPRET) pl.out pl.asm \
	docs/html docs/latex docs/*.aux docs/*.log

$(COMPILER): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(COMPILER) -I $(INCLUDE) \
	$(SRC)/*.cc

$(INTERPRET):
	$(CXX) $(CXXFLAGS) -o $(INTERPRET) $(DRIVER)/driver.cc \
	$(DRIVER)/interp.cc -I $(DRIVER)

.PHONY: docs
docs: $(INCLUDE)
	doxygen docs/doxyfile
	cd docs/latex; make
	cd docs; pdflatex README.tex; pdflatex TechnicalManual.tex
	mv docs/latex/refman.pdf ReferenceDoc.pdf
	mv docs/README.pdf README.pdf
	mv docs/TechnicalManual.pdf TechnicalManual.pdf
