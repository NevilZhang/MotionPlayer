#ifndef _Skeleton_H_
#define _Skeleton_H_

#include "Bone.h"
#define MAX_BONE_NUM			31
class Skeleton {
	public:
		Skeleton() :bone_num(0) {};	//构造函数，默认骨骼数为0
		void set_weight();			//给每一个骨节点设置权重值，离根节点越近越“重”
		void cal_local_coord();     //计算全部骨节点在其父节点的“初始局部坐标系”下的局部坐标
		void cal_global_coord();    //*计算全部骨节点的世界坐标
	public:
		Bone bones[MAX_BONE_NUM];	//核心数据结构，用以存储骨架骨骼数据，通过对象直接访问
		map<string, int> name_to_id;//还是定义为类的成员变量，用于绑定骨骼名字和id的
		int bone_num;				//骨骼总数
};


#endif
