#include "Motion.h"



Motion *mp = new Motion;

int main() {
	mp->Read_ASF("22.asf");
	mp->Read_AMC("22_01.AMC");
	mp->cal_position();                       //重要。计算全部帧的各个骨节点的世界坐标
	system("pause");
	return 0;
}

