#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "calc.hpp"
#include "Edge.hpp"
#include "Vertex.hpp"
#include "Face.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Graph {
private:
	EdgePriorityQueue m_edges;
	VertexSet m_vertexes;
	FaceSet m_faces;
	double m_ratio;
	int m_targetFaceNum;
public:
	// read data from .obj file
	void read(std::string fileName) {
		std::cout << "Reading Data... It will take a while... " << '\n';
		std::ifstream file(fileName);
		std::string line;
		while (getline(file, line))
		{
			if (line.substr(0, 1) == "#") {}
			else if (line.substr(0, 2) == "vt"){}  // texture
			else if (line.substr(0, 2) == "vn"){}  // normal vector
			else if (line.substr(0, 1) == "v") {   // vertex
				//std::cout << "v" << std::endl;
				double x, y, z;
				std::istringstream s(line.substr(2));
				s >> x; s >> y; s >> z;
				this->m_vertexes.addVertex(x, y, z);
			} 
			else if (line.substr(0, 1) == "f") { // face
				int a, b, c;
				std::istringstream s(line.substr(2));
				s >> a; s >> b; s >> c;
				//std::cout << "f " << std::endl;
				a--; b--; c--;
				this->m_vertexes.addNeighor(a, b);
				this->m_vertexes.addNeighor(a, c);
				this->m_vertexes.addNeighor(b, a);
				this->m_vertexes.addNeighor(b, c);
				this->m_vertexes.addNeighor(c, a);
				this->m_vertexes.addNeighor(c, b);

				this->m_faces.addFace(a, b, c);
				
			}
		}
		file.close();


		std::vector<std::pair<int, int>> allVertexes = this->m_vertexes.getAllVertexes();
		for (std::vector<std::pair<int, int>>::iterator it = allVertexes.begin(); it < allVertexes.end(); it++) {
			int v1 = (*it).first; int v2 = (*it).second;
			Edge e(v1, v2);
			calcError(e);
			this->m_edges.addEdge(e);
		}

		std::cout << "Data reading complete. "
			<< "Face Number: " << this->m_faces.getFaceNum() << "  "
			<< "Vertex Number: " << this->m_vertexes.getVertexNum() << "  " << std::endl;
	}

	void setSimpRatio(double ratio) {
		std::cout << "Simplification Ratio is " << ratio << std::endl;
		this->m_ratio = ratio;
		this->m_targetFaceNum = (int)((1 - ratio) * int(this->m_faces.getFaceNum()));
	}

	void write(std::string filename) {
		std::cout << "write to file" << '\n';

		int faceNum = this->m_faces.getFaceNum();
		int vertexNum = this->m_vertexes.getVertexNum();
		std::string content;
		content += "# " + std::to_string(vertexNum) + " " + std::to_string(faceNum) + "\n";
		content += this->m_vertexes.outContent();
		content += this->m_faces.outContent();

		std::ofstream out(filename);
		out << content;
		out.close();
	}

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
		//return v1Coor;
	}

	void calcError(Edge &e) {
		SquareMatrix4D Q = SigmaKp(e.v1) + SigmaKp(e.v2);  // get Sigma Kp of two vertexes of an edge
		e.ECVertex = vertexAfterEC(e, Q);  // calculate the vertex after edge collapse
		Vector4D homoVect(e.ECVertex);  // ECVertex(x, y, z) to homogeneous vector: homoVect(x, y, z, 1)
		Vector4D temp;
		// homoVect*Q*homovect [1*4]*[4*4]*[4*1]
		// temp = homoVect*Q
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4; i++) {
				temp[j] += homoVect[i] * Q[i][j];
			}
		}
		// temp*homoVect
		double err = 0;
		for (int i = 0; i < 4; i++) {
			err += temp[i] * homoVect[i];
		}
		e.err = err;
	}

	void simplify() {
		
		std::cout << "Face number target is " << this->m_targetFaceNum << std::endl;
		std::cout << "Start Simplification" << std::endl;

		while (this->m_faces.getFaceNum() > this->m_targetFaceNum) {
			// get the edge with minimal error
			Edge miniErrEdge;
			bool edgeDeleted = true;
			while (edgeDeleted) {
				miniErrEdge = this->m_edges.getMiniError();
				if (this->m_vertexes.isDeleted(miniErrEdge.v1) ||
					this->m_vertexes.isDeleted(miniErrEdge.v2)) {
					edgeDeleted = true;
				}
				else edgeDeleted = false;
			}

			// delete v1 and v2
			int v1 = miniErrEdge.v1, v2 = miniErrEdge.v2;
			this->m_vertexes.delVertexByIndex(v1);
			this->m_vertexes.delVertexByIndex(v2);

			// make new vertex ECv
			int ECv = this->m_vertexes.addVertex(miniErrEdge.ECVertex);
			// add all neighbors of v1 and v2  to ECv
			this->m_vertexes.addNeighborFromTo(v1, v2, ECv);

			// modiry faces
			this->m_faces.delEdge(v1, v2, ECv);

			// add new edges to edge priority queue
			std::vector<int> ECvNeighbors = this->m_vertexes.getNeighborsByIndex(ECv);
			for (std::vector<int>::iterator it = ECvNeighbors.begin(); it < ECvNeighbors.end(); it++) {
				Edge newEdge(ECv, *it);
				calcError(newEdge);
				this->m_edges.addEdge(newEdge);
			}

			std::cout << "left face number: " << this->m_faces.getFaceNum() << '\n';

		}
		std::cout << "Rearranging index..." <<  '\n';
		std::vector<Old2New> mapping = this->m_vertexes.getOld2NewMapping();
		this->m_faces.reArrange(mapping);
	}
	


};




#endif // !GRAPH_HPP
