#include "VECTOR.h"
#define eps 0.0000000000001
//===================向量相关运算函数定义==================//

//向量归一化函数	normalize_vec
VECTOR normalize_vec(VECTOR &vec) {
	double d = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x /= d;
	vec.y /= d;
	vec.z /= d;
	return vec;
}

//向量夹角函数
double cal_angle(VECTOR &v1, VECTOR &v2){
	double angle;
	double d1 = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
	double d2 = sqrt(v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);
	double d = d1 * d2;
	angle = acos(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z) / d;
	return angle;
}
//计算法向量并归一化
VECTOR cal_normal_vec(VECTOR &v1, VECTOR &v2)
{
	VECTOR v;
	v.x = v1.y*v2.z - v1.z*v2.y;
	v.y = v1.z*v2.x - v1.x*v2.z;
	v.z = v1.x*v2.y - v1.y*v2.x;
	v = normalize_vec(v);
	return v;
}


//======================矩阵运算函数定义======================
//1、矩阵加法
Matrix MatrixPlus(Matrix &A, Matrix &B)
{
	Matrix temp = { 0.0 };
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			temp.Index[i][j] = A.Index[i][j] + B.Index[i][j];
	return temp;
}

//2、矩阵减法
Matrix MatrixSub(Matrix &A, Matrix &B)
{
	Matrix temp = { 0.0 };
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			temp.Index[i][j] = A.Index[i][j] - B.Index[i][j];
	return temp;
}

//3、矩阵A乘矩阵B
Matrix MatrixMult(Matrix &A, Matrix &B)
{
	Matrix temp = { 0.0 };
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			for (int k = 0; k<3; k++)
				temp.Index[i][j] += A.Index[i][k] * B.Index[k][j];
	return temp;
}

//4、矩阵A乘向量B
VECTOR MatrixMultVec(Matrix &A, VECTOR &B)
{
	VECTOR temp;
	double vec[3] = { B.x,B.y,B.z };
	for (int k = 0; k<3; k++)
	{
		temp.x += A.Index[0][k] * vec[k];
		temp.y += A.Index[1][k] * vec[k];
		temp.z += A.Index[2][k] * vec[k];
	}
	return temp;
}

//5、矩阵A求逆。
//
Matrix MatrixInverse(Matrix &A)
{
	Matrix temp = { 0.0 };
	double B[3][6] = { 0.0 };
	//double temp[3][6]={0.0};
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			B[i][j] = A.Index[i][j];
	B[0][3] = 1.0;
	B[1][4] = 1.0;
	B[2][5] = 1.0;

	int first_row = 0;

	double max_element = -1.0;
	for (int i = 0; i<3; i++)
	{
		if (fabs(B[i][0])> max_element)
		{
			max_element = B[i][0];
			first_row = i;
		}
	}

	if (max_element == 0.0)
		cout << "Errors!" << endl;

	//至此，找到了作为第一行的行号；下面将其跟第一行交换位置，使其位于第一行
	if (first_row != 0)
	{
		double temp[1][6] = { 0.0 };
		for (int i = 0; i<6; i++)
		{
			//交换矩阵B中的两行
			temp[0][i] = B[0][i];
			B[0][i] = B[first_row][i];
			B[first_row][i] = temp[0][i];
		}
	}
	//至此，矩阵两行交换完毕；下面将矩阵第一行的第一个元素“归一化”。
	double b00 = B[0][0];
	for (int i = 0; i<6; i++)
		B[0][i] = B[0][i] / b00;
	//下面将第一列后两个元素作减法归零处理
	double b10 = B[1][0];
	double b20 = B[2][0];
	for (int i = 0; i<6; i++)
	{
		B[1][i] -= b10 * B[0][i];
		B[2][i] -= b20 * B[0][i];
	}
	//至此，矩阵第一列为[1,0,0]T
	if (fabs(B[1][1]) <= eps)
	{
		//交换第二行和第三行
		double temp[1][6] = { 0.0 };
		for (int i = 0; i<6; i++)
		{
			//交换矩阵B中的两行
			temp[0][i] = B[1][i];
			B[1][i] = B[2][i];
			B[2][i] = temp[0][i];
		}
	}
	if (fabs(B[1][1]) <= eps) cout << "Errors" << endl;
	//至此，矩阵两行交换完毕；下面将矩阵第二行第二个元素“归一化”。
	double b11 = B[1][1];
	for (int i = 0; i<6; i++)
		B[1][i] /= b11;
	//
	double b21 = B[2][1];
	for (int i = 0; i<6; i++)
		B[2][i] -= b21 * B[1][i];//*改正。
	if (fabs(B[2][2]) <= eps) cout << "Errors" << endl;

	double b22 = B[2][2];
	for (int i = 0; i<6; i++)
		B[2][i] /= b22;
	//至此，上三角矩阵已经形成
	double b01 = B[0][1];
	for (int i = 0; i<6; i++)
		B[0][i] -= b01 * B[1][i];

	double b02 = B[0][2];
	for (int i = 0; i<6; i++)
		B[0][i] -= b02 * B[2][i];

	double b12 = B[1][2];
	for (int i = 0; i<6; i++)
		B[1][i] -= b12 * B[2][i];

	//至此，矩阵B的左半部分已经化为单位矩阵
	//矩阵B的右半部分即为原矩阵A的逆矩阵
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			temp.Index[i][j] = B[i][j + 3];

	return temp;
}

//6、将向量绕X轴逆时针旋转angle角度
VECTOR Matrix_RotateX(double angle, VECTOR &vec)
{
	VECTOR temp;
	Matrix temp1 = { 0.0 };
	//之前要注意转换为弧度制
	temp1.Index[0][0] = 1.0;
	temp1.Index[1][1] = cos(angle);
	temp1.Index[1][2] = -sin(angle);
	temp1.Index[2][1] = sin(angle);
	temp1.Index[2][2] = cos(angle);
	//将矩阵与向量相乘以实现向量的旋转
	temp = MatrixMultVec(temp1, vec);
	//返回变换后的向量
	return temp;
}

//7、将向量绕Y轴逆时针旋转angle角度
VECTOR Matrix_RotateY(double angle, VECTOR &vec)
{
	VECTOR temp;
	Matrix temp1 = { 0.0 };
	//之前要注意转换为弧度制
	temp1.Index[0][0] = cos(angle);
	temp1.Index[0][2] = sin(angle);
	temp1.Index[1][1] = 1.0;
	temp1.Index[2][0] = -sin(angle);
	temp1.Index[2][2] = cos(angle);
	//将矩阵与向量相乘以实现向量的旋转
	temp = MatrixMultVec(temp1, vec);
	//返回变换后的向量
	return temp;
}

//8、将向量绕Z轴逆时针旋转angle角度
VECTOR Matrix_RotateZ(double angle, VECTOR &vec)
{
	VECTOR temp;
	Matrix temp1 = { 0.0 };
	//之前要注意转换为弧度制
	temp1.Index[0][0] = cos(angle);
	temp1.Index[0][1] = -sin(angle);
	temp1.Index[1][0] = sin(angle);
	temp1.Index[1][1] = cos(angle);
	temp1.Index[2][2] = 1.0;
	//将矩阵与向量相乘以实现向量的旋转
	temp = MatrixMultVec(temp1, vec);
	//返回变换后的向量
	return temp;
}





