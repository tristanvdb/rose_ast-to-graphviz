
INSTALL_DIR=/media/ssd/projects/currents/RoseACC-workspace/install_dir/

BOOST_DIR=/media/ssd/boost/install/1_45_0

INC_DIR=../include
LIB_DIR=../lib

INCLUDES=-I$(INC_DIR) -I$(INSTALL_DIR)/include/ -I$(INSTALL_DIR)/include/rose -I$(BOOST_DIR)/include

LIBDIRS=-L$(INSTALL_DIR)/lib -L$(BOOST_DIR)/lib

LIBS=-lrose -lboost_filesystem -lboost_system -lboost_iostreams -lboost_date_time -lboost_wave -lboost_thread -lboost_program_options -lboost_regex

all: whole-ast-to-graphviz extract-api-fragment

check: all
	
clean:
	rm -f whole-ast-to-graphviz.o whole-ast-to-graphviz extract-api-fragment.o extract-api-fragment

############

HEADER_DEPS=$(INC_DIR)/AST2GraphViz/ast-to-graphviz.hpp $(INC_DIR)/AST2GraphViz/default.hpp $(INC_DIR)/AST2GraphViz/cluster.hpp

whole-ast-to-graphviz.o: whole-ast-to-graphviz.cpp $(HEADER_DEPS)
	c++ $(INCLUDES) -g -c whole-ast-to-graphviz.cpp -o whole-ast-to-graphviz.o

whole-ast-to-graphviz: whole-ast-to-graphviz.o $(LIB_DIR)/default.o $(LIB_DIR)/cluster.o
	c++ $(INCLUDES) $(LIBDIRS) $(LIBS) whole-ast-to-graphviz.o $(LIB_DIR)/default.o $(LIB_DIR)/cluster.o -o whole-ast-to-graphviz

extract-api-fragment.o: extract-api-fragment.cpp $(HEADER_DEPS)
	c++ $(INCLUDES) -g -c extract-api-fragment.cpp -o extract-api-fragment.o

extract-api-fragment: extract-api-fragment.o $(LIB_DIR)/default.o $(LIB_DIR)/cluster.o
	c++ $(INCLUDES) $(LIBDIRS) $(LIBS) extract-api-fragment.o $(LIB_DIR)/default.o $(LIB_DIR)/cluster.o -o extract-api-fragment

############

$(LIB_DIR)/default.o: $(LIB_DIR)/default.cpp $(INC_DIR)/AST2GraphViz/ast-to-graphviz.hpp $(INC_DIR)/AST2GraphViz/default.hpp
	make -C $(LIB_DIR) default.o

$(LIB_DIR)/cluster.o: $(LIB_DIR)/cluster.cpp $(HEADER_DEPS)
	make -C $(LIB_DIR) cluster.o


