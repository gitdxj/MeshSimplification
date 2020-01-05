#include "Graph.hpp"
#include "view.h"

int main(int argc, char* argv[]) {
	string srcFilename = argv[1];
	string targetFliename = argv[2];
	double ratio = atof(argv[3]);
	// Mseh Simplification
	Graph g;
	g.setRatio(ratio);
	g.read(srcFilename);
	g.simplify();
	g.write(targetFliename);

	// show OBJ file
	readObj(targetFliename);
	glutInit(&argc, argv);
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(moseMove);
    glutMotionFunc(changeViewPoint);
    glutIdleFunc(myIdle);
    glutMainLoop();
	return 0;
}
