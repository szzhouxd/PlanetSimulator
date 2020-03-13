CXX = $(CROSSPREFIX)g++

PROGRAM = PlanetSimulator.exe
OBJECTS = PlanetSimulator.o
SOURCE = PlanetSimulator.cpp

INCLUDEDIR = -I..\SDL2-2.0.10\include
LIBDIR = -L..\SDL2-2.0.10\lib
LIBS = -lmingw32 -lSDL2main -lSDL2
CXXFLAGS = -O2
LDFLAGS = -s -mwindows

$(PROGRAM) : $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(PROGRAM) $(OBJECTS) $(LIBDIR) $(LIBS)

$(OBJECTS) : $(SOURCE)
	$(CXX) -c $(SOURCE) -o $(OBJECTS) $(CXXFLAGS) $(INCLUDEDIR)

.PHONY : clean

clean :
	-DEL $(PROGRAM) $(OBJECTS)
