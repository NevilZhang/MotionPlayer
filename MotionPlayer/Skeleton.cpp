#include "Skeleton.h"

//��ÿһ���ǽڵ�����Ȩ��ֵ������ڵ�Խ��Ȩ��Խ��
void Skeleton::set_weight()
{
	bones[0].weight = 1.0;
	for (int i = 0; i <= bone_num; i++)
	{
		switch (i)
		{
		case 0:		//��һ�㼶�ĸ��ڵ�
			bones[i].weight = 1.0;
			break;
		case 1:case 6:case 11:
			bones[i].weight = 0.95;
			break;
		case 2:case 7:case 12:case 13:
			bones[i].weight = 0.9;
			break;
		case 3:case 8:case 14:case 17:case 24:
			bones[i].weight = 0.85;
			break;
		case 18:case 25:
			bones[i].weight = 0.8;
			break;
		case 19:case 26:
			bones[i].weight = 0.75;
			break;
		case 4:case 9:case 15:case 27:case 28:
		case 20:case 21:
			bones[i].weight = 0.4;
			break;
		case 5:case 10:case 16:case 29:case 30:
		case 22:case 23:
			bones[i].weight = 0.2;
			break;
		default:
			break;
		}
	}

}

void Skeleton::cal_local_coord(){
	//1����ʼʱ�̣�ÿ������ġ���㡱����һ������������ϵƽ�еġ�ԭʼ�ֲ�����ϵ����
	//�������ȼ���ÿ������ڡ�ԭʼ�ֲ�����ϵ���µ�ԭʼ�ֲ�����������
	//ע1��ÿ���������з���ÿ�������ĳ�ʼ�ֲ�����ϵλ�ڸù�������㴦�ǽڵ㡣
	//ע2������һ�������ֹǽڵ��Ͽ��ܰ󶨶���ֲ�����ϵ�������Ⲣ����������⡣
	for (int i = 1; i <= bone_num; i++)
	{
		bones[i].local_coord.x = bones[i].length*bones[i].direc.x;
		bones[i].local_coord.y = bones[i].length*bones[i].direc.y;
		bones[i].local_coord.z = bones[i].length*bones[i].direc.z;
	}
	//2�����ԭʼ�ֲ�����ϵ����������ת������Ϊ�ù����ġ���ʼ�ֲ�����ϵ����
	//���������Ǽ�����������ڡ���ʼ�ֲ�����ϵ���µ����꣬����ԭʼ�ֲ�����ϵ������z-y-x����ת���γɳ�ʼ�ֲ�����ϵ
	//Ҫ�����ʼ�ֲ���������,�ͽ�ԭʼ�ֲ������������ա���ͬ˳�򡱡�������ת���ɵõ���
	//��ʵ������һ�����������䣬����ϵ��ת��Ȼ�����ԭ������������ϵ�µ��������ļ������⡣���𲽸���������
	for (int i = 1; i <= bone_num; i++)
	{		
		//ע�⣺1���Ƕ�ǰ��Ӹ��ţ�2����ת˳��ӦΪ��z-y-x.
		bones[i].local_coord = Matrix_RotateZ(-bones[i].local_coord_axis.z, bones[i].local_coord);
		bones[i].local_coord = Matrix_RotateY(-bones[i].local_coord_axis.y, bones[i].local_coord);
		bones[i].local_coord = Matrix_RotateX(-bones[i].local_coord_axis.x, bones[i].local_coord);
	}
}

//**�ǳ���Ҫ�����Լ������й�����ȫ�����ꣻ�������ƶ��ǻ������ļ���������������Ϊ�˸����������ڡ�
void Skeleton::cal_global_coord(){
	//1�����ȼ�������ڵ������ʼ�ֲ�����ϵ�µľֲ����ꣻ
	//ע��֮��������������ʼ�ֲ�����ϵ��ת��������������ĳ�ʼ�ֲ�����ϵ����һֱ���䣬������������֮��Ĺ�ϵҲ����仯��
	cal_local_coord();

	//2���ڴ��������֮�ϣ��Ը��������������ֱ任��1������ת�任����2����Ǩ�Ʊ任����
	for (int i = 1; i <= bone_num; i++){
		//��ʱ���������Դ洢ÿһ����������ʼ�ֲ�����ϵ�µľֲ����ꡣ
		VECTOR temp = bones[i].local_coord;
		//��ÿһ���������м��㣬��Ҫ׷�ݵ����ڵ�
		int current_id = i;								//��ǰ����id
		int father_id = bones[current_id].father_id;	//��ǰ�����ĸ��׹���id
		while (current_id != 0)
		{

			//1������ת�任����Χ�Ƶ�ǰ�����ġ���ʼ�ֲ�����ϵ����ת����ת�Ƕ�Ϊ.AMC�ļ��е����ݣ��Ѿ��洢��bones[i].dof��
			//��ʱ�����ʵ�ʱ�Ϊ:����ϵ�̶�������Χ�ƹ̶�����ϵ��ת����µ��������ꡣ
			//*ע��������ѭ����ת˳���ǣ�x-y-z��
			temp = Matrix_RotateX(bones[current_id].dof[0], temp);
			temp = Matrix_RotateY(bones[current_id].dof[1], temp);
			temp = Matrix_RotateZ(bones[current_id].dof[2], temp);

			//2����Ǩ�Ʊ任�������ù����ڵ�ǰ�����ĳ�ʼ�ֲ�����ϵ�µľֲ�����Ǩ�����丸�����ĳ�ʼ�ֲ�����ϵ�µ�����
			//ע��Ǩ�Ʊ任���������ݣ��ռ�����һ�����ƽ�ȵ�����������ϵ��ת������������ϵ��ȥ��
			//���ڹؼ�Ҫ��������ֲ����굽���������ת�����̣���ʼ�ֲ�����ϵ�µľֲ������������Ȼ�ԭ����ԭʼ�ֲ�����ϵ��
			//���ǹؼ��Ĳ��裬֮�����ƽ�Ʊ任����ת������������ϵ�¡�
			//����ɸ���Ϊ���ࣺһ�����ڱ任�������ϵ�µ�����ͨ������ԭ����ǰ������ϵ�µ�������
			//�ڶ����ǣ���ԭ��������ϵ�µ�������˳��������µ�����ϵ�µ�������
			//��ʵ����ͳһ���Ϊ�𲽸�����
			//===========================���ȶ�����ת����==============================
			//������X����ת����
			Matrix temp_current_1 = { 0.0 };
			Matrix temp_father_1 = { 0.0 };

			temp_current_1.Index[0][0] = 1.0;
			temp_current_1.Index[1][1] = cos(bones[current_id].local_coord_axis.x);
			temp_current_1.Index[1][2] = -sin(bones[current_id].local_coord_axis.x);
			temp_current_1.Index[2][1] = sin(bones[current_id].local_coord_axis.x);
			temp_current_1.Index[2][2] = cos(bones[current_id].local_coord_axis.x);

			temp_father_1.Index[0][0] = 1.0;
			temp_father_1.Index[1][1] = cos(bones[father_id].local_coord_axis.x);
			temp_father_1.Index[1][2] = -sin(bones[father_id].local_coord_axis.x);
			temp_father_1.Index[2][1] = sin(bones[father_id].local_coord_axis.x);
			temp_father_1.Index[2][2] = cos(bones[father_id].local_coord_axis.x);

			///������Y����ת����
			Matrix temp_current_2 = { 0.0 };
			Matrix temp_father_2 = { 0.0 };

			temp_current_2.Index[0][0] = cos(bones[current_id].local_coord_axis.y);
			temp_current_2.Index[0][2] = sin(bones[current_id].local_coord_axis.y);
			temp_current_2.Index[1][1] = 1.0;
			temp_current_2.Index[2][0] = -sin(bones[current_id].local_coord_axis.y);
			temp_current_2.Index[2][2] = cos(bones[current_id].local_coord_axis.y);

			temp_father_2.Index[0][0] = cos(bones[father_id].local_coord_axis.y);
			temp_father_2.Index[0][2] = sin(bones[father_id].local_coord_axis.y);
			temp_father_2.Index[1][1] = 1.0;
			temp_father_2.Index[2][0] = -sin(bones[father_id].local_coord_axis.y);
			temp_father_2.Index[2][2] = cos(bones[father_id].local_coord_axis.y);

			///������Z����ת����
			Matrix temp_current_3 = { 0.0 };
			Matrix temp_father_3 = { 0.0 };

			temp_current_3.Index[0][0] = cos(bones[current_id].local_coord_axis.z);
			temp_current_3.Index[0][1] = -sin(bones[current_id].local_coord_axis.z);
			temp_current_3.Index[1][0] = sin(bones[current_id].local_coord_axis.z);
			temp_current_3.Index[1][1] = cos(bones[current_id].local_coord_axis.z);
			temp_current_3.Index[2][2] = 1.0;

			temp_father_3.Index[0][0] = cos(bones[father_id].local_coord_axis.z);
			temp_father_3.Index[0][1] = -sin(bones[father_id].local_coord_axis.z);
			temp_father_3.Index[1][0] = sin(bones[father_id].local_coord_axis.z);
			temp_father_3.Index[1][1] = cos(bones[father_id].local_coord_axis.z);
			temp_father_3.Index[2][2] = 1.0;

			//==============��ת��������ϣ����½�����˹��������ת����==============
			//�����˳����Ȼ����Ҫ

			Matrix rotate_matrix_current = MatrixMult(temp_current_2, temp_current_1);
			rotate_matrix_current = MatrixMult(temp_current_3, rotate_matrix_current);

			Matrix rotate_matrix_father = MatrixMult(temp_father_2, temp_father_1);
			rotate_matrix_father = MatrixMult(temp_father_3, rotate_matrix_father);

			//===========================�����ת��������ϡ�==========================
			//��ʽ��ʼǨ�ƣ�
			//Ǩ�Ʋ���һ��
			temp = MatrixMultVec(rotate_matrix_current, temp);

			//Ǩ�Ʋ������
			//���ȶ���Ǩ��ƽ���������ǻ���ԭʼ�ֲ�����ϵ�µġ�
			VECTOR temp_vec;
			temp_vec.x = bones[father_id].length*bones[father_id].direc.x;
			temp_vec.y = bones[father_id].length*bones[father_id].direc.y;
			temp_vec.z = bones[father_id].length*bones[father_id].direc.z;

			temp.x += temp_vec.x;
			temp.y += temp_vec.y;
			temp.z += temp_vec.z;

			//Ǩ�Ʋ�������
			//�����ֻ�õ���һ�ξ������棬����֮ǰ���Ĵε��ã���߳���Ч�ʡ�
			Matrix temptemp = MatrixInverse(rotate_matrix_father);
			temp = MatrixMultVec(temptemp, temp);

			//���ϣ�ʵ�ֽ���ǰ�����ľֲ�����Ǩ�����丸�����ľֲ�����ϵ�¡�

			//��������ʱ��������ǰ����->���׹��������׹�����>үү������
			current_id = father_id;
			father_id = bones[current_id].father_id;
		}
		//��ʱ��current_id=0,����ʾ���ĸ����ǡ����ڵ㡱�ˡ���ʱ�Ѿ�����Ǩ�������ڵ��ʼ�ֲ�����ϵ�µľֲ������ˡ�

		//������������һ����������ת��Ǩ������������ϵ�£���һ����ƽ���ġ�
		//�����Ǽܵġ���ת�任����������ѭX-Y-Z��˳����С�
		temp = Matrix_RotateX(bones[0].dof[0], temp);
		temp = Matrix_RotateY(bones[0].dof[1], temp);
		temp = Matrix_RotateZ(bones[0].dof[2], temp);
		//�����Ǽܵġ�ƽ�Ʊ任����
		temp.x += bones[0].global_coord.x;
		temp.y += bones[0].global_coord.y;
		temp.z += bones[0].global_coord.z;

		//���ˣ����ֱ任��ϣ�����ȫ������
		bones[i].global_coord = temp;



	}
}
