#include "Bone.h"
Bone::Bone() :
	direc(),
	local_coord(),
	local_coord_axis(),
	global_coord(),
	aligned_coord(),	//新增的对齐坐标
	length(0.0),
	weight(1.0),		//新增的骨节点权值
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