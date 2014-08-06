#####################################
#  MakeFile programma analisi dati  #
#####################################



COMP=g++
FLAGS =
#FLAGS += -DUSE_EXAMPLE
FLAGS += -DSAVE_ALL_HISTOGRAMS
# FLAGS += -DCREATE_OPT_CUT_FILE
ROOTLIBS = `root-config --glibs --cflags` -lMinuit
INC= -I.. -I. -I./include  -I${CLHEP}/include
ROOTINC= -I${ROOTSYS}/include
LIBS= -L.  ${ROOTLIBS} -L${CLHEP}/lib -L${CLHEP}/lib
SRC=
SELECTIONLIB=$(SRC)/rootNtupleClass.o $(SRC)/baseClass.o $(SRC)/analysisClass.o $(SRC)/jsonParser.o $(SRC)/pileupReweighter.o $(SRC)/qcdFitter.o $(SRC)/qcdFitter_V1.o  $(SRC)/likelihoodGett\
er.o $(SRC)/eventListHelper.o
EXE = main


all: digitizerAnalysis graph_noise

digitizerAnalysis: digitizerAnalysis.cpp
	$(COMP) $(INC) $(ROOTINC) $(LIBS)  -o digitizerAnalysis digitizerAnalysis.cpp

graph_noise: graph_noise.cpp
	$(COMP) $(INC) $(ROOTCINT) $(LIBS) -o graph_noise graph_noise.cpp


clean:
	rm -f src/*.o *.lo core core.*
	rm -f *~
	rm -f *.exe
	rm -f $(EXE)

.cpp.o:
	$(COMP) -c $(INC) $(ROOTINC) $(FLAGS) -o $@ $<

.cc.o:
	$(COMP) -m32 -c $(INC) $(ROOTINC) $(FLAGS) -o $@ $<

.cxx.o:
	$(COMP) -c $(INC) $(ROOTINC) $(FLAGS) -o $@ $<

.C.o:
	$(COMP) -c $(INC) $(ROOTINC) $(FLAGS) -o $@ $<