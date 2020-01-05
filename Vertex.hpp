#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>
#include <set>
#include "calc.hpp"

using std::set;

namespace Dongxj {

	class Vertex
	{

	public:
		int id;
		Vector3D coordinate;
		std::set<int> neighbors;

		Vertex() {
			id = -1;
			neighbors.clear();
		}
		Vertex(Vector3D t) {
			id = -1;
			neighbors.clear();
			coordinate = t;
		}
		Vertex(double x, double y, double z) {
			id = -1;
			neighbors.clear();
			coordinate = Vector3D(x, y, z);
		}


		void addNeighbor(int v) {
			neighbors.insert(v);
		}
		void delConnectVertex(int v) {
			neighbors.erase(v);
		}
		bool isExistConnectVertex(int v) {
			return (neighbors.count(v) > 0);
		}
	};


	class VertexSet
	{
	public:
		Vertex m_vertexes[1000000];
		int vertexNum;
		bool isDeleted[1000000];

		VertexSet() {
			vertexNum = 0;
			for (int i = 0; i < 1000000; i++)
				isDeleted[i] = false;
		}

		int addVertex(Vertex v) {
			vertexNum++;
			v.id = vertexNum;
			m_vertexes[vertexNum] = v;
			return vertexNum;
		}
		void delVertex(int _id) {
			if (_id >= 1000000) {
				return;
			}
			isDeleted[_id] = true;

			for (set<int>::iterator it = m_vertexes[_id].neighbors.begin(); it != m_vertexes[_id].neighbors.end(); it++) {
				m_vertexes[(*it)].delConnectVertex(_id);
			}
		}
		int getCommonVertexNum(int u, int v) {
			int cnt = 0;
			for (set<int>::iterator it = m_vertexes[u].neighbors.begin();
				it != m_vertexes[u].neighbors.end(); it++) {
				if (m_vertexes[v].isExistConnectVertex(*it)) {
					cnt++;
				}

			}
			return cnt;
		}
	};
}


#endif
