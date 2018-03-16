#ifndef _Bone_H_
#define _Bone_H_

#include "VECTOR.h"
class Bone {
	public:
		Bone();
	public:
		int		id;						//骨骼id
		string	name;					//骨骼名称
		double	length;					//骨骼长度
		VECTOR  global_coord;			//*全局坐标,绘制时真正用到的就是这个。
		VECTOR  local_coord_axis;		//绑定于该节点的“初始局部坐标系”，重要
		VECTOR  local_coord;            //在其父节点的“初始局部坐标系”下的局部坐标，辅助计算
		VECTOR  direc;					//骨骼初始方向，从父节点到该节点，基于原始坐标系
		int dofflag[3];					//自由度标记数组，用以辅助amc存储
		double dof[3];                  //amc文件里面的旋转角度，用以后续坐标的计算
		vector<int> child_id;           //该骨骼的儿子节点id，但可能有多个儿子
		int father_id;                  //该骨骼的父亲节点id，只可能有一个父亲

		mutable VECTOR  aligned_coord;  //新增，全局坐标经过“空间对齐”之后的坐标；这里居然用到mutable了。
		double weight;				    //*新增。权重值，越靠近根节点权值越大。

};

#endif
