
INSTALL_DIR=/media/ssd/projects/currents/RoseACC-workspace/install_dir/

BOOST_DIR=/media/ssd/boost/install/1_45_0

INC_DIR=../include

INCLUDES=-I$(INC_DIR) -I$(INSTALL_DIR)/include/ -I$(INSTALL_DIR)/include/rose -I$(BOOST_DIR)/include

all: default.o cluster.o

default.o: default.cpp $(INC_DIR)/AST2GraphViz/ast-to-graphviz.hpp $(INC_DIR)/AST2GraphViz/default.hpp
	c++ $(INCLUDES) -g -c default.cpp -o default.o

cluster.o: cluster.cpp $(INC_DIR)/AST2GraphViz/ast-to-graphviz.hpp $(INC_DIR)/AST2GraphViz/default.hpp $(INC_DIR)/AST2GraphViz/cluster.hpp
	c++ $(INCLUDES) -g -c cluster.cpp -o cluster.o

clean:
	rm -f default.o cluster.o

