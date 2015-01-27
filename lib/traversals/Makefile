
VIZ_DIR=../..
include $(VIZ_DIR)/config.mk

LIBS=libVizTraversals.so
TARGETS=empty.o default.o

all: $(LIBS)
	
clean:
	rm -f $(LIBS) $(TARGETS)

############

$(TARGETS): %.o: %.cpp $(VIZ_INC)/Viz/traversal.hpp  $(VIZ_INC)/Viz/Traversals/%.hpp
	c++ $(INCLUDES) -fPIC -g -c $< -o $@

libVizTraversals.so: $(TARGETS)
	c++ -shared $(TARGETS) -o $@

############

