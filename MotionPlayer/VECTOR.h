#ifndef _VECTOR_H_
#define _VECTOR_H_
using namespace std;
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#define EPS 0.00000000000000000001
#define PI	3.14159265358979323846
//==================================================================
//������������
class VECTOR {
public:
	VECTOR() :x(0.0), y(0.0), z(0.0) {}
public:
	double x, y, z;
};
//1��������һ��
VECTOR normalize_vec(VECTOR &vec);

//2���������������ļн�
double cal_angle(VECTOR &v1, VECTOR &v2);

//3���������������ķ�����,��ʹ�����������
VECTOR cal_normal_vec(VECTOR &v1, VECTOR &v2);


//==========================================================
//����������������Լ���ؾ�������
typedef struct
{
	double Index[3][3];
}Matrix;

//1������ӷ�
Matrix MatrixPlus(Matrix &, Matrix &);

//2���������
Matrix MatrixSub(Matrix &, Matrix &);

//3������˷�
Matrix MatrixMult(Matrix &, Matrix &);

//4�����������
VECTOR MatrixMultVec(Matrix &, VECTOR &);

//5����������
Matrix MatrixInverse(Matrix &);

//6����������X����תangle�Ƕ�
VECTOR Matrix_RotateX(double, VECTOR &);

//7����������Y����תangle�Ƕ�
VECTOR Matrix_RotateY(double, VECTOR &);

//8����������Z����תangle�Ƕ�
VECTOR Matrix_RotateZ(double, VECTOR &);


#endif
