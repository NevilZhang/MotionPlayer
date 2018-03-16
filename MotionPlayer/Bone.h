#ifndef _Bone_H_
#define _Bone_H_

#include "VECTOR.h"
class Bone {
	public:
		Bone();
	public:
		int		id;						//����id
		string	name;					//��������
		double	length;					//��������
		VECTOR  global_coord;			//*ȫ������,����ʱ�����õ��ľ��������
		VECTOR  local_coord_axis;		//���ڸýڵ�ġ���ʼ�ֲ�����ϵ������Ҫ
		VECTOR  local_coord;            //���丸�ڵ�ġ���ʼ�ֲ�����ϵ���µľֲ����꣬��������
		VECTOR  direc;					//������ʼ���򣬴Ӹ��ڵ㵽�ýڵ㣬����ԭʼ����ϵ
		int dofflag[3];					//���ɶȱ�����飬���Ը���amc�洢
		double dof[3];                  //amc�ļ��������ת�Ƕȣ����Ժ�������ļ���
		vector<int> child_id;           //�ù����Ķ��ӽڵ�id���������ж������
		int father_id;                  //�ù����ĸ��׽ڵ�id��ֻ������һ������

		mutable VECTOR  aligned_coord;  //������ȫ�����꾭�����ռ���롱֮������ꣻ�����Ȼ�õ�mutable�ˡ�
		double weight;				    //*������Ȩ��ֵ��Խ�������ڵ�ȨֵԽ��

};

#endif
