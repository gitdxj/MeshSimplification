#ifndef EDGE_HPP
#define EDGE_HPP
#include "calc.hpp"
#include <queue>
#include <map>

using std::map;
using std::pair;

namespace Dongxj {

	class Edge
	{
	public:
		int id;
		int v1, v2;
		Vector3D v;
		double error;
		Edge(int v1 = -1, int v2 = -1) {
			id = -1;
			this->v1 = v1;
			this->v2 = v2;
			error = 10000;
		}

	};

	class EdgePriorityQueue
	{
	public:
		EdgePriorityQueue(void) {
			cntEdge = 0;
			for (int i = 0; i < 1000000; i++)
				isDeleted[i] = false;
		}
		// specify the definition of priority in priotiry queue
		struct cmp {
			bool operator() (Edge e1, Edge e2) {
				return e1.error > e2.error;
			}
		};
		std::priority_queue<Edge, std::vector<Edge>, cmp> pq;
		map<pair<int, int>, int> mapEdgeToID;
		bool isDeleted[1000000];
		int cntEdge;
		void addEdge(Edge& e) {
			cntEdge++;
			e.id = cntEdge;
			int u = std::min(e.v1, e.v2);
			int v = std::max(e.v1, e.v2);
			mapEdgeToID[std::make_pair(u, v)] = cntEdge;
			pq.push(e);
		}
		void delEdge(Edge e) {
			int u = std::min(e.v1, e.v2);
			int v = std::max(e.v1, e.v2);
			int ID = mapEdgeToID[std::make_pair(u, v)];
			isDeleted[ID] = true;
		};
		Edge getMinErr() {
			if (pq.size() <= 0) {
				return Edge(0, 0);
			}
			while (isDeleted[pq.top().id]) {
				pq.pop();
			}
			Edge e = pq.top();
			pq.pop();
			return e;
		}
	};
}


#endif
