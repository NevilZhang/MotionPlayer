#include "VECTOR.h"
#define eps 0.0000000000001
//===================����������㺯������==================//

//������һ������	normalize_vec
VECTOR normalize_vec(VECTOR &vec) {
	double d = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	vec.x /= d;
	vec.y /= d;
	vec.z /= d;
	return vec;
}

//�����нǺ���
double cal_angle(VECTOR &v1, VECTOR &v2){
	double angle;
	double d1 = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
	double d2 = sqrt(v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);
	double d = d1 * d2;
	angle = acos(v1.x*v2.x + v1.y*v2.y + v1.z*v2.z) / d;
	return angle;
}
//���㷨��������һ��
VECTOR cal_normal_vec(VECTOR &v1, VECTOR &v2)
{
	VECTOR v;
	v.x = v1.y*v2.z - v1.z*v2.y;
	v.y = v1.z*v2.x - v1.x*v2.z;
	v.z = v1.x*v2.y - v1.y*v2.x;
	v = normalize_vec(v);
	return v;
}


//======================�������㺯������======================
//1������ӷ�
Matrix MatrixPlus(Matrix &A, Matrix &B)
{
	Matrix temp = { 0.0 };
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			temp.Index[i][j] = A.Index[i][j] + B.Index[i][j];
	return temp;
}

//2���������
Matrix MatrixSub(Matrix &A, Matrix &B)
{
	Matrix temp = { 0.0 };
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			temp.Index[i][j] = A.Index[i][j] - B.Index[i][j];
	return temp;
}

//3������A�˾���B
Matrix MatrixMult(Matrix &A, Matrix &B)
{
	Matrix temp = { 0.0 };
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			for (int k = 0; k<3; k++)
				temp.Index[i][j] += A.Index[i][k] * B.Index[k][j];
	return temp;
}

//4������A������B
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

//5������A���档
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

	//���ˣ��ҵ�����Ϊ��һ�е��кţ����潫�����һ�н���λ�ã�ʹ��λ�ڵ�һ��
	if (first_row != 0)
	{
		double temp[1][6] = { 0.0 };
		for (int i = 0; i<6; i++)
		{
			//��������B�е�����
			temp[0][i] = B[0][i];
			B[0][i] = B[first_row][i];
			B[first_row][i] = temp[0][i];
		}
	}
	//���ˣ��������н�����ϣ����潫�����һ�еĵ�һ��Ԫ�ء���һ������
	double b00 = B[0][0];
	for (int i = 0; i<6; i++)
		B[0][i] = B[0][i] / b00;
	//���潫��һ�к�����Ԫ�����������㴦��
	double b10 = B[1][0];
	double b20 = B[2][0];
	for (int i = 0; i<6; i++)
	{
		B[1][i] -= b10 * B[0][i];
		B[2][i] -= b20 * B[0][i];
	}
	//���ˣ������һ��Ϊ[1,0,0]T
	if (fabs(B[1][1]) <= eps)
	{
		//�����ڶ��к͵�����
		double temp[1][6] = { 0.0 };
		for (int i = 0; i<6; i++)
		{
			//��������B�е�����
			temp[0][i] = B[1][i];
			B[1][i] = B[2][i];
			B[2][i] = temp[0][i];
		}
	}
	if (fabs(B[1][1]) <= eps) cout << "Errors" << endl;
	//���ˣ��������н�����ϣ����潫����ڶ��еڶ���Ԫ�ء���һ������
	double b11 = B[1][1];
	for (int i = 0; i<6; i++)
		B[1][i] /= b11;
	//
	double b21 = B[2][1];
	for (int i = 0; i<6; i++)
		B[2][i] -= b21 * B[1][i];//*������
	if (fabs(B[2][2]) <= eps) cout << "Errors" << endl;

	double b22 = B[2][2];
	for (int i = 0; i<6; i++)
		B[2][i] /= b22;
	//���ˣ������Ǿ����Ѿ��γ�
	double b01 = B[0][1];
	for (int i = 0; i<6; i++)
		B[0][i] -= b01 * B[1][i];

	double b02 = B[0][2];
	for (int i = 0; i<6; i++)
		B[0][i] -= b02 * B[2][i];

	double b12 = B[1][2];
	for (int i = 0; i<6; i++)
		B[1][i] -= b12 * B[2][i];

	//���ˣ�����B����벿���Ѿ���Ϊ��λ����
	//����B���Ұ벿�ּ�Ϊԭ����A�������
	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			temp.Index[i][j] = B[i][j + 3];

	return temp;
}

//6����������X����ʱ����תangle�Ƕ�
VECTOR Matrix_RotateX(double angle, VECTOR &vec)
{
	VECTOR temp;
	Matrix temp1 = { 0.0 };
	//֮ǰҪע��ת��Ϊ������
	temp1.Index[0][0] = 1.0;
	temp1.Index[1][1] = cos(angle);
	temp1.Index[1][2] = -sin(angle);
	temp1.Index[2][1] = sin(angle);
	temp1.Index[2][2] = cos(angle);
	//�����������������ʵ����������ת
	temp = MatrixMultVec(temp1, vec);
	//���ر任�������
	return temp;
}

//7����������Y����ʱ����תangle�Ƕ�
VECTOR Matrix_RotateY(double angle, VECTOR &vec)
{
	VECTOR temp;
	Matrix temp1 = { 0.0 };
	//֮ǰҪע��ת��Ϊ������
	temp1.Index[0][0] = cos(angle);
	temp1.Index[0][2] = sin(angle);
	temp1.Index[1][1] = 1.0;
	temp1.Index[2][0] = -sin(angle);
	temp1.Index[2][2] = cos(angle);
	//�����������������ʵ����������ת
	temp = MatrixMultVec(temp1, vec);
	//���ر任�������
	return temp;
}

//8����������Z����ʱ����תangle�Ƕ�
VECTOR Matrix_RotateZ(double angle, VECTOR &vec)
{
	VECTOR temp;
	Matrix temp1 = { 0.0 };
	//֮ǰҪע��ת��Ϊ������
	temp1.Index[0][0] = cos(angle);
	temp1.Index[0][1] = -sin(angle);
	temp1.Index[1][0] = sin(angle);
	temp1.Index[1][1] = cos(angle);
	temp1.Index[2][2] = 1.0;
	//�����������������ʵ����������ת
	temp = MatrixMultVec(temp1, vec);
	//���ر任�������
	return temp;
}





