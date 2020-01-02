#ifndef CALC_H
#define CALC_H

struct Vector3D {
	float x, y, z;
	bool operator==(const Vector3D &v) {
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}
};

#endif // !CALC_H