#ifndef CALC_HPP
#define CALC_HPP

#include <cmath>
#include <iostream>
#include <iomanip>  // 用来格式化输出

using std::cout;
using std::endl;

namespace Dongxj {

	class Vector3D
	{

	public:
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

		Vector3D operator + (const Vector3D &V) {
			return Vector3D(x + V.x, y + V.y, z + V.z);
		}
		Vector3D operator - (const Vector3D &V) {
			return Vector3D(x - V.x, y - V.y, z - V.z);
		}
		Vector3D operator / (double t) {
			return Vector3D(x / t, y / t, z / t);
		}

		Vector3D getUnitVectorOfThis() {
			double len = sqrt(x * x + y * y + z * z);
			return *this / len;
		}

	};

	Vector3D crossProduct(Vector3D a, Vector3D b) {
		return Vector3D(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}

	double dotProduct(Vector3D a, Vector3D b) {
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	class Vector4D
	{
	public:
		double data[4];
		Vector4D(double X = 0, double Y = 0, double Z = 0, double W = -99) {
			data[0] = X; data[1] = Y; data[2] = Z; data[3] = W;
		}
		double& operator[](int i) {
			return data[i];
		}
		Vector4D operator + (const Vector4D& V) {
			return Vector4D(data[0] + V.data[0], data[1] + V.data[1], data[2] + V.data[2], data[3] + V.data[3]);
		}
	};


	class SquareMatrix4D
	{
	public:
		double data[4][4];

		double* operator[](int i) {

			return data[i];
		}

		SquareMatrix4D() {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					data[i][j] = 0;
				}
			}
		}

		SquareMatrix4D operator +(const SquareMatrix4D &M) {
			SquareMatrix4D m;
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					m.data[i][j] = data[i][j] + M.data[i][j];
			return m;
		}


		void show() {
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++) {
					cout << std::fixed/*以小数形式输出（不用科学计数法）*/
						<< std::setprecision(1)/*保留小数点后一位*/
						<< std::setw(6)/*指定输出宽度为6，不足用空格补齐*/
						<< std::right/*向右对齐*/ << data[i][j];
				}
				cout << endl;
			}
			cout << endl;
		}
	};

}
#endif


