#include "Bone.h"
Bone::Bone() :
	direc(),
	local_coord(),
	local_coord_axis(),
	global_coord(),
	aligned_coord(),	//�����Ķ�������
	length(0.0),
	weight(1.0),		//�����Ĺǽڵ�Ȩֵ
	id(0),
	father_id(0)
{
	dof[0] = 0.0;
	dof[1] = 0.0;
	dof[2] = 0.0;
	dofflag[0] = 0;
	dofflag[1] = 0;
	dofflag[2] = 0;
}