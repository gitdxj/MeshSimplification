#ifndef FACE_HPP
#define FACE_HPP

#include<vector>
#include<utility>
#include"Vertex.hpp"
struct Face {
	int a, b, c;
	int index;
	bool isDeleted;
	Face(int p_a, int p_b, int p_c, int p_index = -1) {
		a = p_a; b = p_b; c = p_c; isDeleted = false;
	}
	bool hasVertex(int v) {
		return (a == v) || (b == v) || (c == v);
	}
};

class FaceSet {
private:
	std::vector<Face> m_faces;
	int nextFaceIndex;
	int numFace;
public:
	FaceSet() { nextFaceIndex = 0; numFace = 0; }
	void addFace(int a, int b, int c) {
		this->m_faces.push_back(Face(a, b, c, nextFaceIndex));
		nextFaceIndex++;
		numFace++;
	}
	void addFace(Face f) {
		this->m_faces.push_back(Face(f));
		numFace++;
	}
	std::vector<Face> findVertexInSameFace(int vertexIndex) {
		std::vector<Face> faces;
		for (std::vector<Face>::iterator it = m_faces.begin(); it < m_faces.end(); it++) {
			if ((*it).hasVertex(vertexIndex)) {
				faces.push_back(Face(*it));
			}
		}
		return faces;
	}
	int getFaceNum() {
		return numFace;
	}
	void delEdge(int v1, int v2, int ECv) {
		// if a face has one of v1 and v2 then repalce v1 to ECv
		for (std::vector<Face>::iterator it = this->m_faces.begin(); it < m_faces.end(); it++) {
			if ((*it).isDeleted) {
				continue;
			}
			bool hasV1 = (*it).hasVertex(v1);
			bool hasV2 = (*it).hasVertex(v2);
			if (hasV1 || hasV2) {
				if (hasV1 && hasV2) {  // has both v1 and v2
					(*it).isDeleted = true;
					std::cout << "delete face with edge " << v1 << ' ' << v2 << std::endl;
					numFace--;
				}
				else if(hasV1){  // replace v1 with ECv
					if (v1 == (*it).a) {
						(*it).a = ECv;
					}
					else if (v1 == (*it).b) {
						(*it).b = ECv;
					}
					else if (v1 == (*it).c) {
						(*it).c = ECv;
					}
				}
				else if (hasV2) { // replace v2 with ECv
					if (v2 == (*it).a) {
						(*it).a = ECv;
					}
					else if (v2 == (*it).b) {
						(*it).b = ECv;
					}
					else if (v2 == (*it).c) {
						(*it).c = ECv;
					}
				}
			}
		}
		// if a face has both v1 and v2, delete this face
	}
	//void changeFromOld2New(int &v, Old2New o2n) {}

	void reArrange(std::vector<Old2New> mapping){
		for (std::vector<Face>::iterator it = m_faces.begin(); it < m_faces.end(); it++) {
			Old2New o2n;
			if (findInMapping(mapping, (*it).a, o2n)) {
				(*it).a = o2n.second;
			}
			if (findInMapping(mapping, (*it).b, o2n)) {
				(*it).b = o2n.second;
			}
			if (findInMapping(mapping, (*it).c, o2n)) {
				(*it).c = o2n.second;
			}
		}
	}

	std::string outContent() {
		std::string content;
		for (std::vector<Face>::iterator it = this->m_faces.begin(); it < this->m_faces.end(); it++) {
			if (!(*it).isDeleted) {
				content += "f " + std::to_string((*it).a+1) + " " + std::to_string((*it).b=1) + " " + std::to_string((*it).c+1) + "\n";
			}
		}
		return content;
	}
};

#endif // !FACE_HPP