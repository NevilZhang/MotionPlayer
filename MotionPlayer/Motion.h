#ifndef _Motion_H_
#define _Motion_H_
#include "Skeleton.h"

class Motion {
	public:
		Motion();
		void Read_ASF(string filename);		//读取ASF骨架数据
		void Read_AMC(string filename);		//读取AMC运动数据
		void cal_position();				//计算每一帧的骨架信息
	public:
		Skeleton skeletons[5000];			//骨架对象预设上限5000帧
		int total_frame_num;                //该运动片段总帧数。
};
#endif
