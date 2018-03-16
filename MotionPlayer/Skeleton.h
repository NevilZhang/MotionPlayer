#ifndef _Skeleton_H_
#define _Skeleton_H_

#include "Bone.h"
#define MAX_BONE_NUM			31
class Skeleton {
	public:
		Skeleton() :bone_num(0) {};	//���캯����Ĭ�Ϲ�����Ϊ0
		void set_weight();			//��ÿһ���ǽڵ�����Ȩ��ֵ������ڵ�Խ��Խ���ء�
		void cal_local_coord();     //����ȫ���ǽڵ����丸�ڵ�ġ���ʼ�ֲ�����ϵ���µľֲ�����
		void cal_global_coord();    //*����ȫ���ǽڵ����������
	public:
		Bone bones[MAX_BONE_NUM];	//�������ݽṹ�����Դ洢�Ǽܹ������ݣ�ͨ������ֱ�ӷ���
		map<string, int> name_to_id;//���Ƕ���Ϊ��ĳ�Ա���������ڰ󶨹������ֺ�id��
		int bone_num;				//��������
};


#endif
