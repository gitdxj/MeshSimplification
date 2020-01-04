#ifndef EDGE_HPP
#define EDGE_HPP
#include <queue>
#include "calc.hpp"

class Edge {
public:
	int v1, v2; // v1 and v2 are indexes of vertex
	Vector3D ECVertex; // vertex we get after edge collapse
	bool isDeleted;
	double err;
	Edge() { v1 = -1; v2 = -1; }
	Edge(int p_v1, int p_v2) { v1 = p_v1; v2 = p_v2;}
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
	std::priority_queue<Edge, std::vector<Edge>, cmp> m_priorityQueue; // In a priority queue, element with the highest priority will be placed on the root
	int cntEdge;
public:
	EdgePriorityQueue() {}
	void addEdge(Edge e) {
		Edge newEdge(e);
		this->m_priorityQueue.push(newEdge);
	}
	Edge getMiniError(){
		if (this->m_priorityQueue.empty()) {
			return Edge(-1, -1);
		}
		Edge targetEdge(this->m_priorityQueue.top());
		this->m_priorityQueue.pop();
		return targetEdge;
	}
};

#endif // !EDGE_HPP