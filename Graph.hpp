#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "calc.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"
#include "Face.hpp"
#include <string>
class Graph {
private:
	EdgePriorityQueue m_edges;
	VertexSet m_vertexes;
	FaceSet m_faces;
	int m_ratio, numFaces;
public:
	// read data from .obj file
	void read(std::string fileName) {}
	void setSimpRatio(double) {}
	void simplify(){}
	void write(std::string filename) {}

	SquareMatrix4D SigmaKp(int vertexIndex) {
		SquareMatrix4D sigmaKp;
		Vector3D aCoor;
		this->m_vertexes.getVertexCoorByIndex(vertexIndex, aCoor);
		std::vector<Face> faces = this->m_faces.findVertexInSameFace(vertexIndex);
		// Traversal all faces that have vertexIndex
		for (std::vector<Face>::iterator it = faces.begin(); it < faces.end(); it++) {
			Face f = *it;
			// allocate the other tow vertexes as bIndex and cIndex
			int bIndex, cIndex;
			if (f.a == vertexIndex) {
				bIndex = f.b; cIndex = f.c;
			}
			else if (f.b == vertexIndex) {
				bIndex = f.a; cIndex = f.c;
			}
			else if (f.c == vertexIndex) {
				bIndex = f.a; cIndex = f.b;
			}
			Vector3D bCoor, cCoor;
			this->m_vertexes.getVertexCoorByIndex(bIndex, bCoor);
			this->m_vertexes.getVertexCoorByIndex(cIndex, cCoor);
			// calculate the normal vector of this face
			Vector3D normal = crossProduct(bCoor - aCoor, cCoor - aCoor).getUnitVector();
			// ax+by+cz+d=0, normal vector is (a, b, c)
			double d = -dotProduct(normal, aCoor); // ax+by+cz=-d, get d by replacing (x, y, z) with any vertex
			double factor[4] = { normal.x, normal.y, normal.z, d };
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					sigmaKp[i][j] += factor[i] * factor[j];
				}
			}
		}
		return sigmaKp;
	}
	
	// calculate the vertex after Edge Collapse
	Vector3D vertexAfterEC(Edge &e, SquareMatrix4D m) {
		Vector3D v1Coor, v2Coor;
		this->m_vertexes.getVertexCoorByIndex(e.v1, v1Coor);
		this->m_vertexes.getVertexCoorByIndex(e.v2, v2Coor);
		Vector3D middlePoint((v1Coor + v2Coor) / 2);
		return middlePoint;
	}

	void calcError(Edge &e) {
		SquareMatrix4D Q = SigmaKp(e.v1) + SigmaKp(e.v2);
		e.ECVertex = vertexAfterEC(e, Q);
		Vector4D homoVect(e.ECVertex);
		double err = 0;
		for (int i = 0; i < 4; i++) {
			double p = 0;
			for (int j = 0; j < 4; j++)
				p += homoVect[j] * Q[i][j];
			err += p * homoVect[i];
		}
		e.err = err;
	}

};




#endif // !GRAPH_HPP
