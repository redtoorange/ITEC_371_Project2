CXX = g++
CXXFLAGS = -std=c++11 -Wall

OBJECTS =   FSObject.o Directory.o File.o ProgramFile.o TextFile.o main.o

all: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -static-libstdc++ -o RUFS main.o FSObject.o Directory.o File.o ProgramFile.o TextFile.o

FSObject.o:
	g++ $(CXXFLAGS) -c FSObject.cpp

Directory.o:
	g++ $(CXXFLAGS) -c Directory.cpp

File.o:
	g++ $(CXXFLAGS) -c File.cpp

ProgramFile.o:
	g++ $(CXXFLAGS) -c ProgramFile.cpp

TextFile.o:
	g++ $(CXXFLAGS) -c TextFile.cpp

main.o:
	g++ $(CXXFLAGS) -c main.cpp
