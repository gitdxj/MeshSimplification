#ifndef VERTEX_H
#define VERTEX_H

#include<iostream>
#include<vector>
using namespace std;

class Vertex {
public:
	int x, y, z;
	int index;
	vector<int> m_neighbors;
	bool m_isDeleted;  // if this vertex is deleted, set this to true

	// constructor
	Vertex() { x = 0; y = 0; z = 0; index = -1; m_isDeleted = false; }
	Vertex(int p_x, int p_y, int p_z, int p_index = -1) {
		this->x = p_x; this->y = p_y; this->z = p_z; this->index = p_index; m_isDeleted = false;
	}

	// two vertexes are the same if their coordinates are of the same
	bool operator==(Vertex p) {
		if (p.x == x && p.y == y && p.z == z) {
			return true;
		}
		else {
			return false;
		}
	}
	// add a neighbor of this vertex
	void addNeighbor(int index) {
		for (vector<int>::iterator it = m_neighbors.begin(); it < m_neighbors.end(); it++) {
			if (*it == index) {
				return;
			}
			m_neighbors.push_back(index);
		}

	}
	// delete a neighbor of vertex 
	void deleteNeighbor(int index) {
		for (vector<int>::iterator it = m_neighbors.begin(); it < m_neighbors.end(); it++) {
			if (*it == index) {
				m_neighbors.erase(it);
				return;
			}
		}
	}

	// add all the neighbors of another vertex to this vertex
	void addNeighborFromVertex(Vertex v) {
		clearNeighbors();
		for (vector<int>::iterator it = v.m_neighbors.begin(); it < m_neighbors.end(); it++) {
			this->m_neighbors.push_back(*it);
		}

	}

	// clear all neighbors of this vertex
	void clearNeighbors() {
		m_neighbors.clear();
	}

};

class VertexSet {
private:
	vector<Vertex> m_vertexes;
	int m_nextVertexIndex;
	bool findVertexByCord(int x, int y, int z, vector<Vertex>::iterator &position) {
		for (vector<Vertex>::iterator it = m_vertexes.begin();
			it < m_vertexes.end(); it++) {
			if (((*it).x == x) && ((*it).y == y) && ((*it).z == z)) {
				position = it;
				return true;
			}
		}
		return false;
	}
	bool findVertexByIndex(int index, vector<Vertex>::iterator &position) {
		for (vector<Vertex>::iterator it = m_vertexes.begin();
			it < m_vertexes.end(); it++) {
			if ((*it).index == index) {
				position = it;
				return true;
			}
		}
		return false;
	}
public:
	VertexSet() { m_nextVertexIndex = 0; }
	bool delVertexByCord(int x, int y, int z) {
		vector<Vertex>::iterator position;
		if (findVertexByCord(x, y, z, position)) {
			m_vertexes.erase(position);
			return true;
		}
		else return false;
	}

	void delVertexByIndex(int index) {
		vector<Vertex>::iterator position;
		if (findVertexByIndex(index, position)) {
			m_vertexes.erase(position);
		}
	}
	bool addVertex(Vertex v) {
		vector<Vertex>::iterator position;
		// if there is no vertex has the same coordinate, add the new vertex to m_vertexes
		if (!findVertexByCord(v.x, v.y, v.z, position)) {
			Vertex newVertex(v);
			newVertex.index = m_nextVertexIndex;
			m_vertexes.push_back(newVertex);
			m_nextVertexIndex++;
			return true;
		}
		else return false;
	}
};

#endif // !VERTEX_H