#include "Motion.h"



Motion *mp = new Motion;

int main() {
	mp->Read_ASF("22.asf");
	mp->Read_AMC("22_01.AMC");
	mp->cal_position();                       //��Ҫ������ȫ��֡�ĸ����ǽڵ����������
	system("pause");
	return 0;
}

