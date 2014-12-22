
INSTALL_DIR=/media/ssd/projects/currents/RoseACC-workspace/install_dir/

BOOST_DIR=/media/ssd/boost/install/1_45_0

INCLUDES=-I$(INSTALL_DIR)/include/ -I$(INSTALL_DIR)/include/rose -I$(BOOST_DIR)/include

LIBDIRS=-L$(INSTALL_DIR)/lib -L$(BOOST_DIR)/lib

LIBS=-lrose -lboost_filesystem -lboost_system -lboost_iostreams -lboost_date_time -lboost_wave -lboost_thread -lboost_program_options -lboost_regex

all: whole-ast-to-graphviz extract-api-fragment

ast-to-graphviz.o: ast-to-graphviz.cpp ast-to-graphviz.hpp
	c++ $(INCLUDES) -c ast-to-graphviz.cpp -o ast-to-graphviz.o

whole-ast-to-graphviz.o: whole-ast-to-graphviz.cpp ast-to-graphviz.hpp
	c++ $(INCLUDES) -c whole-ast-to-graphviz.cpp -o whole-ast-to-graphviz.o

whole-ast-to-graphviz: whole-ast-to-graphviz.o ast-to-graphviz.o
	c++ $(INCLUDES) $(LIBDIRS) $(LIBS) whole-ast-to-graphviz.o ast-to-graphviz.o -o whole-ast-to-graphviz

extract-api-fragment.o: extract-api-fragment.cpp ast-to-graphviz.hpp
	c++ $(INCLUDES) -c extract-api-fragment.cpp -o extract-api-fragment.o

extract-api-fragment: extract-api-fragment.o ast-to-graphviz.o
	c++ $(INCLUDES) $(LIBDIRS) $(LIBS) extract-api-fragment.o ast-to-graphviz.o -o extract-api-fragment
	

