deps = calc.hpp Edge.hpp Vertex.hpp Graph.hpp view.h

MeshSimp: main.cpp view.cpp $(deps)
	g++ main.cpp view.cpp -o MeshSimp -lGL -lGLU -lglut

clean:
	rm MeshSimp

run:
	./MeshSimp horse.obj simplified_horse.obj 0.01

