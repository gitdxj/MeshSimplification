#ifndef EDGE_H
#define EDGE_H
#include <cstdlib>
#include <cstdio>
//���࣬���ڱ�ʾһ����
class Edge
{
public:
	Edge(int _u = -99, int _v = -99);
	~Edge(void);
	int id;//�����
	int v1, v2;//�˵�ı��
	MyVec3d v;//�������γɵ�
	double deltaV;//�������ɵ�v�Ĵ���
};
#endif // !EDGE_H