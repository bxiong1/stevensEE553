CXX	:=	g++
DEBUG	:=     #	-g

FRACOBJ	:= Cube.o Cylinder.o main.o
Shape: $(FRACOBJ)
	$(CXX) $(DEBUG) $(FRACOBJ) -o Shape

Cube.o: Cube.cc Cube.hh
	$(CXX) $(DEBUG) -c $<

Cylinder.o: Cylinder.cc Cylinder.hh
	$(CXX) $(DEBUG) -c $<

main.o: main.cc Cylinder.hh Cube.hh
	$(CXX) $(DEBUG) -c $<


