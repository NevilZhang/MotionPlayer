#ifndef _Motion_H_
#define _Motion_H_
#include "Skeleton.h"

class Motion {
	public:
		Motion();
		void Read_ASF(string filename);		//��ȡASF�Ǽ�����
		void Read_AMC(string filename);		//��ȡAMC�˶�����
		void cal_position();				//����ÿһ֡�ĹǼ���Ϣ
	public:
		Skeleton skeletons[5000];			//�Ǽܶ���Ԥ������5000֡
		int total_frame_num;                //���˶�Ƭ����֡����
};
#endif
