#ifndef VERTEX_HPP
#define VERTEX_HPP
#define MAX_VERTEX 10000
#include<iostream>
#include<vector>
#include<string>
#include<utility> // std::pair
#include"calc.hpp"

// typedef pair<int, int> FaceNode;
typedef std::pair<int, int> Old2New; // a mapping from old vertex index to new vertex index
bool findInMapping(std::vector<Old2New> mapping, int v, Old2New &o2n) {
	for (std::vector<Old2New>::iterator it = mapping.begin(); it < mapping.end(); it++) {
		if (v == (*it).first) {
			o2n = *it;
			return true;
		}
	}
	return false;
}

class Vertex {
public:
	Vector3D m_coordinate;
	int index;
	std::vector<int> m_neighbors;
	// vector<FaceNode> m_faceCombo; // every FaceNode element has two vertex index, these two vertexes and the vertex itself make up a face
	bool isDeleted;  // if this vertex is deleted, set this to true
	// constructor
	Vertex() { m_coordinate.x = 0; m_coordinate.y = 0; m_coordinate.z = 0; index = -1; isDeleted = false;  }
	Vertex(double p_x, double p_y, double p_z, int p_index = -1) {
		this->m_coordinate.x = p_x; 
		this->m_coordinate.y = p_y; 
		this->m_coordinate.z = p_z; 
		this->index = p_index; isDeleted = false;
	}
	Vertex(Vector3D p_coor, int p_index = -1) {
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
	void addNeighbor(int v) {
		bool existsV = false;
		for (std::vector<int>::iterator id = m_neighbors.begin(); id < m_neighbors.end(); id++) {
			if (*id == v)
				existsV = true;
		}
		if(!existsV)
			m_neighbors.push_back(v);
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

	//// visit private elements
	//double getX() { return this->m_coordinate.x; }
	//double getY() { return this->m_coordinate.y; }
	//double getZ() { return this->m_coordinate.z; }
	//Vector3D getCoordinate() { return this->m_coordinate; }
	//int getIndex() { return this->index; }

	//// set private elements
	//void setIndex(int index) { this->index = index; }
};

class VertexSet {
private:
	Vertex m_vertexes[MAX_VERTEX];
	std::vector<int> m_indexes;
	bool isDefault[MAX_VERTEX];
	int m_nextVertexIndex;
	int m_vertexNum;

	bool getVertex(int index, Vertex &v) {
		if (!isDefault[index]) {
			v = m_vertexes[index];
			return true;
		}
		else return false;
	}
public:
	VertexSet() { m_nextVertexIndex = 0; m_vertexNum = 0; 
	for (int i = 0; i < MAX_VERTEX; i++) isDefault[i] = true;}

	int getVertexNum() { return this->m_vertexNum; }

	void delVertexByIndex(int index) {
		//std::vector<Vertex>::iterator position;
		//if (findVertexByIndex(index, position)) {
		//	m_vertexes.erase(position);
		//}
		this->m_vertexes[index].isDeleted = true;
		this->m_vertexNum--;
	}
	bool isDeleted(int index) {
		return this->m_vertexes[index].isDeleted;
	}

	int addVertex(Vertex v) {
		isDefault[m_nextVertexIndex] = false;
		this->m_vertexes[m_nextVertexIndex] = v;
		this->m_indexes.push_back(m_nextVertexIndex);
		m_nextVertexIndex++;
		this->m_vertexNum++;  // number of vertexes
		return m_nextVertexIndex - 1;
	}

	int addVertex(double x, double y, double z) {
		Vertex newVertex(x, y, z);
		return addVertex(newVertex);
	}

	int addVertex(Vector3D vect) {
		return addVertex(vect.x, vect.y, vect.z);
	}

	void addNeighor(int v, int neighborIndex) {
		if (!isDefault[v]) {
			this->m_vertexes[v].addNeighbor(neighborIndex);
		}
	}
	
	void addNeighborFromTo(int v1, int v2, int ECv) { // add the neighbors of v1 and v2 to ECv
		std::vector<int> neighbor1 = m_vertexes[v1].m_neighbors;
		std::vector<int> neighbor2 = m_vertexes[v2].m_neighbors;
		for (std::vector<int>::iterator it = neighbor1.begin(); it < neighbor1.end(); it++) {
			if (*it != v2) { // add all v1's neighbors except v2
				m_vertexes[ECv].addNeighbor(*it);
			}
		}
		for (std::vector<int>::iterator it = neighbor2.begin(); it < neighbor2.end(); it++) {
			if (*it != v1) { // add all v2's neighbors except v1
				m_vertexes[ECv].addNeighbor(*it);
			}
		}
	}

	std::vector<int> getNeighborsByIndex(int v) {
		return m_vertexes[v].m_neighbors;
	}

	bool getVertexCoorByIndex(int v, Vector3D &coor) {
		if (!isDefault[v]) {
			coor = m_vertexes[v].m_coordinate;
			return true;
		}
		else return false;
	}

	// get all vertexes by examinating the neighbors of every vertex
	std::vector<std::pair<int, int>> getAllVertexes() {
		std::vector<std::pair<int, int>> allVertexes;
		for (std::vector<int>::iterator id = this->m_indexes.begin(); id < this->m_indexes.end(); id++) {
			if (!this->m_vertexes[*id].isDeleted) {
				std::vector<int> neighbors = this->m_vertexes[*id].m_neighbors;
				for (std::vector<int>::iterator it = neighbors.begin(); it < neighbors.end(); it++) {
					if (*id > *it) {
						continue;
					}
					allVertexes.push_back(std::pair<int, int>(*id, *it));
				}
			}
		}
		return allVertexes;
	}

	std::vector<Old2New> getOld2NewMapping() {
		std::vector<Old2New> oldNewMapping;
		int nextIndex = 0;
		for (std::vector<int>::iterator id = this->m_indexes.begin(); id < this->m_indexes.end(); id++) {
			if (!m_vertexes[*id].isDeleted) {
				oldNewMapping.push_back(Old2New(*id, nextIndex));
				nextIndex++;
			}
		}
		return oldNewMapping;
	}

	std::string outContent() {
		std::string content;
		for (std::vector<int>::iterator id = this->m_indexes.begin(); id < this->m_indexes.end(); id++) {
			if (!m_vertexes[*id].isDeleted) {
				content += "v " + std::to_string((m_vertexes[*id]).m_coordinate.x) + " " + std::to_string((m_vertexes[*id]).m_coordinate.y)
					+ " " +std::to_string((m_vertexes[*id]).m_coordinate.z) + "\n";
			}
		}
		return content;
	}
};

#endif // !VERTEX_H