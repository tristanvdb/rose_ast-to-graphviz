
VIZ_DIR=..
include $(VIZ_DIR)/config.mk

BINS=empty-viz default-viz
TARGETS=empty-viz.o default-viz.o

all: $(BINS)

check: check-empty check-default
	
clean:
	rm -f $(BINS) $(TARGETS)
	rm -f *.dot *.svg

############

empty-viz.o: viz.cpp $(VIZ_INC)/Viz/traversal.hpp $(VIZ_INC)/Viz/Traversals/empty.hpp
	c++ $(INCLUDES) -g -DVIZ_TRAVERSAL=0 -c $< -o $@

default-viz.o: viz.cpp $(VIZ_INC)/Viz/traversal.hpp $(VIZ_INC)/Viz/Traversals/default.hpp
	c++ $(INCLUDES) -g -DVIZ_TRAVERSAL=1 -c $< -o $@

programmable-viz.o: viz.cpp $(VIZ_INC)/Viz/traversal.hpp $(VIZ_INC)/Viz/Traversals/programmable.hpp
	c++ $(INCLUDES) -g -DVIZ_TRAVERSAL=2 -c $< -o $@

$(BINS): %: %.o $(VIZ_LIB)/traversals/libVizTraversals.so
	c++ $(INCLUDES) $(LIBDIRS) $(LIBS) $< -o $@

############

%.svg: %.dot
	dot -Tsvg $< -o $@

check-empty: empty_test_01.svg empty_test_02.svg

empty_%.dot: $(VIZ_EXAMPLES)/%.c empty-viz
	LD_LIBRARY_PATH=$(VIZ_LD_LIBRARY_PATH):$(LD_LIBRARY_PATH) ./empty-viz -prefix empty_ -DSKIP_ROSE_BUILTIN_DECLARATIONS $<

check-default: default_test_01.svg default_test_02.svg

default_%.dot: $(VIZ_EXAMPLES)/%.c default-viz
	LD_LIBRARY_PATH=$(VIZ_LD_LIBRARY_PATH):$(LD_LIBRARY_PATH) ./default-viz -prefix default_ -DSKIP_ROSE_BUILTIN_DECLARATIONS $<

check-programmable: program_0_test_01.svg program_0_test_02.svg program_1_test_01.svg program_1_test_02.svg

program_0_%.dot: $(VIZ_EXAMPLES)/%.c programmable-viz program_0.viz
	LD_LIBRARY_PATH=$(VIZ_LD_LIBRARY_PATH):$(LD_LIBRARY_PATH) ./programmable-viz -viz_program program_0.viz -prefix program_0_ -DSKIP_ROSE_BUILTIN_DECLARATIONS $<

program_1_%.dot: $(VIZ_EXAMPLES)/%.c programmable-viz program_1.viz
	LD_LIBRARY_PATH=$(VIZ_LD_LIBRARY_PATH):$(LD_LIBRARY_PATH) ./programmable-viz -viz_program program_1.viz -prefix program_1_ -DSKIP_ROSE_BUILTIN_DECLARATIONS $<

############

$(VIZ_LIB)/traversals/libVizTraversals.so:
	make -C $(VIZ_LIB)/traversals libVizTraversals.so

############

