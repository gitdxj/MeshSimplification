#ifndef CALC_HPP
#define CALC_HPP
#include<cmath>
#include<algorithm>
// 3 dimentional vector
struct Vector3D {
	double x, y, z;
	Vector3D() {}
	Vector3D(double p_x, double p_y, double p_z) {
		x = p_x; y = p_y; z = p_z;
	}
	bool operator==(const Vector3D &v) {
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}
	double& operator[](int i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw "IndexOutOfRange";
		}
	}
	Vector3D operator-(const Vector3D &vect) {
		return Vector3D(x - vect.x, y - vect.y, z - vect.z);
	}

	Vector3D operator+(const Vector3D &vect) {
		return Vector3D(x + vect.x, y + vect.y, z + vect.z);
	}

	Vector3D operator/(double denomi) {
		return Vector3D(x / denomi, y / denomi, z / denomi);
	}

	// dot product
	double operator*(const Vector3D &vect) {
		return x * vect.x + y * vect.y + z * vect.z;
	}

	Vector3D getUnitVector() {
		double squredLen = x*x + y*y + z*z;
		double denominator = sqrt(squredLen);
		return Vector3D(x / denominator, y / denominator, z / denominator);
	}
};

// the cross product of two vectors are vertical to these two vectors
Vector3D crossProduct(Vector3D a, Vector3D b) {
	return Vector3D(b.y*a.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

double dotProduct(Vector3D a, Vector3D b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector3D getUnitVector(Vector3D vect) {
	double squredLen = dotProduct(vect, vect);
	double denominator = sqrt(squredLen);
	return Vector3D(vect.x / denominator, vect.y / denominator, vect.z / denominator);
}


// for homogeneous vector
struct Vector4D {
	double x, y, z, w;
	Vector4D() { x = y = z = w = 0; }
	// Initialize a homogeneous vector using a 3 dimentianal vector
	Vector4D(Vector3D vect) {
		this->x = vect.x;
		this->y = vect.y;
		this->z = vect.z;
		this->w = 1;
	}
	double& operator[](int i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw "IndexOutOfRange";
		}
	}
};

class SquareMatrix4D {
public:
	double m_data[4][4];
	SquareMatrix4D() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m_data[i][j] = 0;
			}
		}
	}
	// enable accessing the element by var_name[i][j]
	double* operator[](int i) {
		return this->m_data[i];
	}

	SquareMatrix4D operator+(const SquareMatrix4D &m) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m_data[i][j] = m_data[i][j] + m.m_data[i][j];
			}
		}
		return *this;
	}
	
};


// solve linear equation set
Vector4D solveEquation(SquareMatrix4D m, Vector4D v) {
	Vector4D result;
	return result;
}


#endif // !CALC_HPP