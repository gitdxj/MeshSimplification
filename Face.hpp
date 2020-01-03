#include<vector>
#include<utility>
struct Face {
	int a, b, c;
	int index;
	bool isDeleted;
	Face(int p_a, int p_b, int p_c, int p_index = -1) {
		a = p_a; b = p_b; c = p_c; isDeleted = false;
	}
	bool hasVertex(int f) {
		return (a == f) || (b == f) || (c == f);
	}
};

typedef std::pair<int, int> Old2New; // a mapping from old vertex index to new vertex index

class FaceSet {
private:
	std::vector<Face> m_faces;
	int nextFaceIndex;
public:
	FaceSet() { nextFaceIndex = 0; }
	void addFace(int a, int b, int c) {
		this->m_faces.push_back(Face(a, b, c, nextFaceIndex));
		nextFaceIndex++;
	}
	void addFace(Face f) {
		this->m_faces.push_back(Face(f));
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
	void reArrange(std::vector<Old2New>){
		for (std::vector<Face>::iterator it = m_faces.begin(); it < m_faces.end(); it++) {

		}
	}
};