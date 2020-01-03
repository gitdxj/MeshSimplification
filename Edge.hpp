#ifndef EDGE_HPP
#define EDGE_HPP
#include <queue>
#include "calc.hpp"

class Edge {
public:
	int v1, v2; // v1 and v2 are indexes of vertex
	int index;
	Vector3D ECVertex; // vertex we get after edge collapse
	bool m_isDeleted;
	double err;
	Edge() { v1 = -1; v2 = -1; index = -1; }
	Edge(int p_v1, int p_v2, int p_index = -1) { v1 = p_v1; v2 = p_v2; index = p_index; }
	//double getError() { return this->m_error; }
};



class EdgePriorityQueue
{
private:
	// specify the standard of priority in priority queue
	struct cmp {
		bool operator() (Edge e1, Edge e2) {
			return e2.err > e1.err;
		}
	};
	std::priority_queue<Edge, std::vector<Edge>, cmp> pq; // In a priority queue, element with the highest priority will be placed on the root
	int cntEdge;
public:
	EdgePriorityQueue() {}
	void addEdge(Edge e){}
	void deleteEdge(Edge e){}
	Edge getMiniError(){}
};

#endif // !EDGE_HPP