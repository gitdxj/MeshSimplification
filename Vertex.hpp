#ifndef VERTEX_HPP
#define VERTEX_HPP

#include<iostream>
#include<vector>
#include<utility> // std::pair
#include"calc.hpp"

// typedef pair<int, int> FaceNode;

class Vertex {
private:
	Vector3D m_coordinate;
	int index;
	std::vector<int> m_neighbors;
	// vector<FaceNode> m_faceCombo; // every FaceNode element has two vertex index, these two vertexes and the vertex itself make up a face
	bool m_isDeleted;  // if this vertex is deleted, set this to true
public:
	// constructor
	Vertex() { m_coordinate.x = 0; m_coordinate.y = 0; m_coordinate.z = 0; index = -1; m_isDeleted = false; }
	Vertex(double p_x, double p_y, double p_z, int p_index = -1) {
		this->m_coordinate.x = p_x; 
		this->m_coordinate.y = p_y; 
		this->m_coordinate.z = p_z; 
		this->index = p_index; m_isDeleted = false;
	}
	Vertex(Vector3D p_coor, int p_index) {
		this->m_coordinate = p_coor;
		this->index = p_index;
	}

	// two vertexes are the same if their coordinates are of the same
	bool operator==(Vertex p) {
		if (p.m_coordinate.x == m_coordinate.x && p.m_coordinate.y == m_coordinate.y && p.m_coordinate.z == m_coordinate.z) {
			return true;
		}
		else {
			return false;
		}
	}
	// add a neighbor of this vertex
	void addNeighbor(int index) {
		for (std::vector<int>::iterator it = m_neighbors.begin(); it < m_neighbors.end(); it++) {
			if (*it == index) {
				return;
			}
			m_neighbors.push_back(index);
		}

	}
	// delete a neighbor of vertex 
	void deleteNeighbor(int index) {
		for (std::vector<int>::iterator it = m_neighbors.begin(); it < m_neighbors.end(); it++) {
			if (*it == index) {
				m_neighbors.erase(it);
				return;
			}
		}
	}

	// add all the neighbors of another vertex to this vertex
	void addNeighborFromVertex(Vertex v) {
		clearNeighbors();
		for (std::vector<int>::iterator it = v.m_neighbors.begin(); it < m_neighbors.end(); it++) {
			this->m_neighbors.push_back(*it);
		}

	}

	// clear all neighbors of this vertex
	void clearNeighbors() {
		m_neighbors.clear();
	}

	// visit private elements
	double getX() { return this->m_coordinate.x; }
	double getY() { return this->m_coordinate.y; }
	double getZ() { return this->m_coordinate.z; }
	Vector3D getCoordinate() { return this->m_coordinate; }
	int getIndex() { return this->index; }

	// set private elements
	void setIndex(int index) { this->index = index; }
};

class VertexSet {
private:
	std::vector<Vertex> m_vertexes;
	int m_nextVertexIndex;
	bool findVertexByCord(Vector3D vect, std::vector<Vertex>::iterator &position) {
		for (std::vector<Vertex>::iterator it = m_vertexes.begin();
			it < m_vertexes.end(); it++) {
			if ((*it).getCoordinate() == vect) {
				position = it;
				return true;
			}
		}
		return false;
	}
	bool findVertexByIndex(int index, std::vector<Vertex>::iterator &position) {
		for (std::vector<Vertex>::iterator it = m_vertexes.begin();
			it < m_vertexes.end(); it++) {
			if ((*it).getIndex() == index) {
				position = it;
				return true;
			}
		}
		return false;
	}
public:
	VertexSet() { m_nextVertexIndex = 0; }
	bool delVertexByCord(Vector3D vect) {
		std::vector<Vertex>::iterator position;
		if (findVertexByCord(vect, position)) {
			m_vertexes.erase(position);
			return true;
		}
		else return false;
	}

	void delVertexByIndex(int index) {
		std::vector<Vertex>::iterator position;
		if (findVertexByIndex(index, position)) {
			m_vertexes.erase(position);
		}
	}
	bool addVertex(Vertex v) {
		std::vector<Vertex>::iterator position;
		// if there is no vertex has the same coordinate, add the new vertex to m_vertexes
		if (!findVertexByCord(v.getCoordinate(), position)) {
			Vertex newVertex(v);
			newVertex.setIndex(m_nextVertexIndex);
			m_vertexes.push_back(newVertex);
			m_nextVertexIndex++;
			return true;
		}
		else return false;
	}
	
	bool getVertexCoorByIndex(int vertexIndex, Vector3D &coor) {
		std::vector<Vertex>::iterator v;
		if (findVertexByIndex(vertexIndex, v)) {
			coor = (*v).getCoordinate();
			return true;
		}
		else return false;
	}

};

#endif // !VERTEX_H