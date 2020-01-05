#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Edge.hpp"
#include "Vertex.hpp"
#include "calc.hpp"
#include <string>
#include <fstream>
#include <sstream>

using std::string;
using std::set;
using std::to_string;
using Dongxj::EdgePriorityQueue;
using Dongxj::VertexSet;
using Dongxj::Vertex;
using Dongxj::Edge;
using Dongxj::Vector3D;
using Dongxj::Vector4D;
using Dongxj::SquareMatrix4D;


class Graph
{
private:
	double ratio;
	int faceNum, delFaceNum;
	EdgePriorityQueue* epq; 
	VertexSet* vertexes; // all vertexes
public:
	Graph(void) {
		epq = new EdgePriorityQueue();
		vertexes = new VertexSet();
		faceNum = 0;
	}

	void setRatio(double ratio) {
		this->ratio = ratio;
	}

	void read(string fileName) {
		std::cout << "Reading Data... It will take a while... " << '\n';
		std::ifstream file(fileName);
		string line;
		int vNum = 0, fNum = 0;
		int tmp = 0;
		while (getline(file, line))
		{
			if (line.substr(0, 1) == "#") {}
			else if (line.substr(0, 2) == "vt") {}  // texture
			else if (line.substr(0, 2) == "vn") {}  // normal vector
			else if (line.substr(0, 1) == "v") {   // vertex
				vNum++;
				double x, y, z;
				std::istringstream s(line.substr(2));
				s >> x; s >> y; s >> z;
				vertexes->addVertex(Vertex(x, y, z));
			}
			else if (line.substr(0, 1) == "f") { // face
				fNum++;
				faceNum++;
				int a, b, c;
				std::istringstream s(line.substr(2));
				s >> a; s >> b; s >> c;
				// add neighbors according to face 
				vertexes->m_vertexes[a].addNeighbor(b);  
				vertexes->m_vertexes[a].addNeighbor(c);
				vertexes->m_vertexes[b].addNeighbor(a);
				vertexes->m_vertexes[b].addNeighbor(c);
				vertexes->m_vertexes[c].addNeighbor(a);
				vertexes->m_vertexes[c].addNeighbor(b);
			}
		}
		cout << "Data all read: " << vNum << " vertexes and " << fNum << " faces " << endl;
		
		// add edges to edge priority queue
		for (int i = 1; i <= vertexes->vertexNum; i++) {
			for (set<int>::iterator it = vertexes->m_vertexes[i].neighbors.begin();
				it != vertexes->m_vertexes[i].neighbors.end(); it++) {
				if (i < (*it)) break;
				Edge e((*it), i);
				calEdgeError(e);
				epq->addEdge(e);
			}
		}
		delFaceNum = (int)((1 - ratio) * faceNum);
	}



	void simplify() {
		cout << "Now start edge collapsing" << endl;
		for (int i = 0; i < delFaceNum; i += 2/*cause 2 faces will disapear when 1 edge is deleted*/) {
			Edge e = epq->getMinErr();
			Vertex* v1 = &(vertexes->m_vertexes[e.v1]);
			Vertex* v2 = &(vertexes->m_vertexes[e.v2]);
			Vertex ECv = e.v; // ECv is the vetex we get after edge collapse
			int ECvId = vertexes->addVertex(ECv);

			set<int> neighbors;
			neighbors.clear();
			epq->delEdge(e);  // delete edge e

			// delete all neighbors of v1 and add all these neighbors to a neighbor set
			for (set<int>::iterator it = v1->neighbors.begin(); it != v1->neighbors.end(); it++) {
				if ((*it) != v2->id) {
					epq->delEdge(Edge((*it), v1->id));
					vertexes->m_vertexes[(*it)].delConnectVertex(v1->id);
					neighbors.insert((*it));
				}

			}
			// do the same to v2
			for (set<int>::iterator it = v2->neighbors.begin(); it != v2->neighbors.end(); it++) {
				if ((*it) != v1->id) {
					epq->delEdge(Edge((*it), v2->id));
					vertexes->m_vertexes[(*it)].delConnectVertex(v2->id);
					neighbors.insert((*it));
				}

			}

			// add all the neighbors of v1 and v2 to ECv
			for (set<int>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
				vertexes->m_vertexes[(*it)].addNeighbor(ECvId);
				vertexes->m_vertexes[ECvId].addNeighbor(*it);
			}
			// delete v1 and v2
			vertexes->isDeleted[v1->id] = true;
			vertexes->isDeleted[v2->id] = true;

			// make new edges according to neighbors we add to ECv, and add these edges to edge priority queue
			for (set<int>::iterator it = neighbors.begin(); it != neighbors.end(); it++) {
				Edge e((*it), ECvId);
				calEdgeError(e);
				epq->addEdge(e);
			}



		}
	}


	void write(string filename) {
		cout << "Write vertexes and faces to target file" << endl;
		string content;
		int index = 0; // new index of vertexes
		// write all the vertexes
		for (int i = 1; i <= vertexes->vertexNum; i++) {
			if (vertexes->isDeleted[i])
				continue;
			Vertex* v = &vertexes->m_vertexes[i];
			index++;
			v->id = index;
			content += "v " + to_string(v->coordinate.x) + ' ' + to_string(v->coordinate.y) + ' '
				+ to_string(v->coordinate.z) + '\n';
		}
		// write all the faces
		for (int i = 1; i <= vertexes->vertexNum; i++) {
			if (vertexes->isDeleted[i])
				continue;
			Vertex* v = &(vertexes->m_vertexes[i]);
			for (set<int>::iterator it1 = v->neighbors.begin(); it1 != v->neighbors.end(); it1++) {
				if (i >= (*it1))
					continue;
				for (set<int>::iterator it2 = v->neighbors.begin(); it2 != v->neighbors.end(); it2++) {
					if ((*it1) < (*it2) && (vertexes->m_vertexes[(*it1)].isExistConnectVertex(*it2))) {
						content += "f " + to_string(v->id) + ' ' + to_string(vertexes->m_vertexes[(*it1)].id) + ' '
							+ to_string(vertexes->m_vertexes[(*it2)].id) + '\n';
					}
				}
			}
		}
		std::ofstream out(filename);
		out << content;
		out.close();
		cout << "All Done" << endl;
	}


	SquareMatrix4D SigmaKp(int v) {
		SquareMatrix4D result;
		Vertex* v0 = &(vertexes->m_vertexes[v]);
		for (set<int>::iterator it1 = v0->neighbors.begin(); it1 != v0->neighbors.end(); it1++) {
			for (set<int>::iterator it2 = v0->neighbors.begin(); it2 != v0->neighbors.end(); it2++) {
				if ((*it1) < (*it2) && (vertexes->m_vertexes[(*it1)].isExistConnectVertex(*it2))) {
					Vertex* v1 = &(vertexes->m_vertexes[(*it1)]);
					Vertex* v2 = &(vertexes->m_vertexes[(*it2)]);
					Vector3D n = crossProduct((v1->coordinate) - (v0->coordinate), (v2->coordinate) - (v0->coordinate)).getUnitVectorOfThis();
					// tmp is (a, b, c, d), the plane(v0, v1, v0) is algebrally ax+by+cz+d;
					Vector4D tmp(n.x, n.y, n.z, -dotProduct(v1->coordinate, n));  // apply any one of v0, v1, v2 to equation d = -(ax+by+cz)
					/***********************************
								|a^2 ab  ac  ad |
						  Sigma |ab  a^2 bc  bd |
								|ac  bc  c^2 cd |
								|ad  bd  cd  d^2|
					************************************/
					for (int i = 0; i < 4; i++) {
						for (int j = 0; j < 4; j++) {
							result[i][j] += tmp[i] * tmp[j];
						}
					}
				}
			}
		}
		return result;
	}
	Vector3D getECVertexCoordinate(Edge& e, SquareMatrix4D m) {
		// just usr middle point
		return (vertexes->m_vertexes[e.v1].coordinate + vertexes->m_vertexes[e.v2].coordinate) / 2; 
	}
	void calEdgeError(Edge& e) {
		SquareMatrix4D Q = SigmaKp(e.v1) + SigmaKp(e.v2);  // get Sigma Kp of two vertexes of an edge
		e.v = getECVertexCoordinate(e, Q);  // calculate the vertex after edge collapse
		Vector4D homoVect(e.v.x, e.v.y, e.v.z, 1.0);  // ECVertex(x, y, z) to homogeneous vector: homoVect(x, y, z, 1)
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
		e.error = err;

	}
};


#endif // !GRAPH_HPP