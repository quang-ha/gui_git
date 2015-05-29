OPENGL_LIBS = -lglut -lGL -lGLU

CXX = $(shell wx-config --cxx)

SRC = logsim.cc gui.cc names.cc name_ex.cc scan_ex.cc parse_ex.cc scanner.cc parser.cc network.cc monitor.cc devices.cc interpret.cc 

L_OBJECTS = logsim.o names.o scanner.o network.o parser.o monitor.o devices.o gui.o parse_ex.o scan_ex.o interpret.o name_ex.o

# implementation

.SUFFIXES:	.o .cc

.cc.o :
	$(CXX) -c `wx-config --cxxflags` -g -o $@ $<

all:    logsim 

logsim:	$(L_OBJECTS)
	$(CXX) -o logsim $(L_OBJECTS) `wx-config --libs --gl_libs` $(OPENGL_LIBS)

clean: 
	rm -f *.o logsim 

depend:
	makedepend $(SRC)

# DO NOT DELETE

logsim.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
names.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
name_ex.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
network.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
parser.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
parse_ex.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
scanner.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h  parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
scan_ex.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h  parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
monitor.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h  parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
devices.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h  parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
gui.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h  parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 
interpret.o: logsim.h names.h devices.h network.h monitor.h parser.h gui.h  parse_ex.h scan_ex.h name_ex.h interpret.h scanner.h 




