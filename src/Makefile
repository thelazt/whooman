BIN := bomber

CXX := g++
LINKER := g++
INCDIRS := -I.
LIBS := -lSDL -lSDL_image
CXXFLAGS := -std=c++14 -Wall -Wextra -Og -g3 -ggdb

SRCFILES := $(wildcard *.cc)
OBJFILES := $(patsubst %.cc,%.o,$(SRCFILES))
DEPFILES := $(patsubst %.cc,%.d,$(SRCFILES))

$(BIN): $(OBJFILES)
	$(LINKER) $^ $(LIBS) -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(LIBS) $(INCDIRS) -c $< -o $@

%.d: %.cc
	$(CXX) $(INCDIRS) $(LIBS) -MM $< > $@

install:
	echo "No install"

clean:
	rm -f *.o *.d

-include $(DEPFILES)

.PHONY: install clean run
