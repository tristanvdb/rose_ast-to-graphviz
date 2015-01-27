
VIZ_DIR=..
include $(VIZ_DIR)/config.mk

BINS=empty default
TARGETS=empty.o default.o

all: $(BINS)

check: check-empty check-default
	
clean:
	rm -f $(BINS) $(TARGETS)
	rm -f *.dot *.svg

############

$(TARGETS): %.o: %.cpp ../include/Viz/traversal.hpp
	c++ $(INCLUDES) -g -c $< -o $@

$(BINS): %: %.o
	c++ $(INCLUDES) $(LIBDIRS) $(LIBS) $< -o $@

############

check-empty: empty_test_01.svg empty_test_02.svg

empty_%.dot: $(VIZ_EXAMPLES)/%.c empty
	./empty -prefix empty_ -DSKIP_ROSE_BUILTIN_DECLARATIONS $<

check-default: default_test_01.svg default_test_02.svg

default_%.dot: $(VIZ_EXAMPLES)/%.c default
	./default -prefix default_ -DSKIP_ROSE_BUILTIN_DECLARATIONS $<

%.svg: %.dot
	dot -Tsvg $< -o $@

############

