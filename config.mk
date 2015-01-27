
ROSE_DIR?=/media/ssd/projects/currents/RoseACC-workspace/install_dir/
BOOST_DIR?=/media/ssd/boost/install/1_45_0/

VIZ_INC=$(VIZ_DIR)/include
VIZ_LIB=$(VIZ_DIR)/lib
VIZ_EXAMPLES=$(VIZ_DIR)/examples

BOOST_LIBS=-lboost_filesystem -lboost_system -lboost_iostreams -lboost_date_time -lboost_wave -lboost_thread -lboost_program_options -lboost_regex

ROSE_LIBS=-lrose
DLX_LIBS=-lDLX-core
MFB_LIBS=-lMFB-sage
MDCG_LIBS=-lMDCG
KLT_LIBS=-lKLT -lKLT-core

INCLUDES=-I$(VIZ_INC) -I$(ROSE_DIR)/include/ -I$(ROSE_DIR)/include/rose -I$(BOOST_DIR)/include

LIBDIRS=-L$(ROSE_DIR)/lib -L$(BOOST_DIR)/lib

LIBS=$(BOOST_LIBS) $(ROSE_LIBS) $(DLX_LIBS) $(MFB_LIBS) $(MDCG_LIBS) $(KLT_LIBS)

