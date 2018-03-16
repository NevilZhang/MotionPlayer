#include "Skeleton.h"

//给每一个骨节点设置权重值，离根节点越近权重越大
void Skeleton::set_weight()
{
	bones[0].weight = 1.0;
	for (int i = 0; i <= bone_num; i++)
	{
		switch (i)
		{
		case 0:		//第一层级的根节点
			bones[i].weight = 1.0;
			break;
		case 1:case 6:case 11:
			bones[i].weight = 0.95;
			break;
		case 2:case 7:case 12:case 13:
			bones[i].weight = 0.9;
			break;
		case 3:case 8:case 14:case 17:case 24:
			bones[i].weight = 0.85;
			break;
		case 18:case 25:
			bones[i].weight = 0.8;
			break;
		case 19:case 26:
			bones[i].weight = 0.75;
			break;
		case 4:case 9:case 15:case 27:case 28:
		case 20:case 21:
			bones[i].weight = 0.4;
			break;
		case 5:case 10:case 16:case 29:case 30:
		case 22:case 23:
			bones[i].weight = 0.2;
			break;
		default:
			break;
		}
	}

}

void Skeleton::cal_local_coord(){
	//1、初始时刻，每块骨骼的“起点”处绑定一个与世界坐标系平行的“原始局部坐标系”；
	//我们首先计算每块骨骼在“原始局部坐标系”下的原始局部坐标向量；
	//注1：每个骨骼都有方向；每个骨骼的初始局部坐标系位于该骨骼的起点处骨节点。
	//注2：这样一来，部分骨节点上可能绑定多个局部坐标系，不过这并不会带来问题。
	for (int i = 1; i <= bone_num; i++)
	{
		bones[i].local_coord.x = bones[i].length*bones[i].direc.x;
		bones[i].local_coord.y = bones[i].length*bones[i].direc.y;
		bones[i].local_coord.z = bones[i].length*bones[i].direc.z;
	}
	//2、随后，原始局部坐标系经过——旋转——成为该骨骼的“初始局部坐标系”；
	//接下来我们计算各个骨骼在“初始局部坐标系”下的坐标，由于原始局部坐标系依次绕z-y-x轴旋转而形成初始局部坐标系
	//要计算初始局部坐标向量,就将原始局部坐标向量按照“相同顺序”“反向”旋转即可得到。
	//这实质上是一个：向量不变，坐标系旋转，然后计算原向量在新坐标系下的新向量的几何问题。“逐步跟进”策略
	for (int i = 1; i <= bone_num; i++)
	{		
		//注意：1、角度前面加负号；2、旋转顺序应为：z-y-x.
		bones[i].local_coord = Matrix_RotateZ(-bones[i].local_coord_axis.z, bones[i].local_coord);
		bones[i].local_coord = Matrix_RotateY(-bones[i].local_coord_axis.y, bones[i].local_coord);
		bones[i].local_coord = Matrix_RotateX(-bones[i].local_coord_axis.x, bones[i].local_coord);
	}
}

//**非常重要。用以计算所有骨骼的全局坐标；后续绘制都是基于它的计算结果；其余计算均为了辅助它而存在。
void Skeleton::cal_global_coord(){
	//1、首先计算各个节点在其初始局部坐标系下的局部坐标；
	//注：之后各个骨骼绕其初始局部坐标系旋转，假设各个骨骼的初始局部坐标系方向一直不变，这样以来它们之间的关系也不会变化。
	cal_local_coord();

	//2、在此坐标基础之上，对各个骨骼进行两种变换：1、“旋转变换”；2、“迁移变换”。
	for (int i = 1; i <= bone_num; i++){
		//临时变量，用以存储每一块骨骼在其初始局部坐标系下的局部坐标。
		VECTOR temp = bones[i].local_coord;
		//对每一个骨骼进行计算，都要追溯到根节点
		int current_id = i;								//当前骨骼id
		int father_id = bones[current_id].father_id;	//当前骨骼的父亲骨骼id
		while (current_id != 0)
		{

			//1、“旋转变换”，围绕当前骨骼的“初始局部坐标系”旋转，旋转角度为.AMC文件中的数据，已经存储在bones[i].dof中
			//此时问题的实质变为:坐标系固定，向量围绕固定坐标系旋转求解新的向量坐标。
			//*注：这里遵循的旋转顺序是：x-y-z。
			temp = Matrix_RotateX(bones[current_id].dof[0], temp);
			temp = Matrix_RotateY(bones[current_id].dof[1], temp);
			temp = Matrix_RotateZ(bones[current_id].dof[2], temp);

			//2、“迁移变换”，将该骨骼在当前骨骼的初始局部坐标系下的局部坐标迁移至其父骨骼的初始局部坐标系下的坐标
			//注：迁移变换的理论依据：空间任意一点可以平等的在两套坐标系下转换到世界坐标系中去。
			//现在关键要厘清骨骼局部坐标到世界坐标的转换过程，初始局部坐标系下的局部坐标向量首先还原至在原始局部坐标系下
			//这是关键的步骤，之后进行平移变换即可转换至世界坐标系下。
			//问题可概括为两类：一类是在变换后的坐标系下的向量通过逆序还原到以前的坐标系下的向量；
			//第二类是：在原来的坐标系下的向量逐步顺序跟进到新的坐标系下的向量。
			//其实可以统一理解为逐步跟进。
			//===========================首先定义旋转矩阵==============================
			//定义绕X轴旋转矩阵：
			Matrix temp_current_1 = { 0.0 };
			Matrix temp_father_1 = { 0.0 };

			temp_current_1.Index[0][0] = 1.0;
			temp_current_1.Index[1][1] = cos(bones[current_id].local_coord_axis.x);
			temp_current_1.Index[1][2] = -sin(bones[current_id].local_coord_axis.x);
			temp_current_1.Index[2][1] = sin(bones[current_id].local_coord_axis.x);
			temp_current_1.Index[2][2] = cos(bones[current_id].local_coord_axis.x);

			temp_father_1.Index[0][0] = 1.0;
			temp_father_1.Index[1][1] = cos(bones[father_id].local_coord_axis.x);
			temp_father_1.Index[1][2] = -sin(bones[father_id].local_coord_axis.x);
			temp_father_1.Index[2][1] = sin(bones[father_id].local_coord_axis.x);
			temp_father_1.Index[2][2] = cos(bones[father_id].local_coord_axis.x);

			///定义绕Y轴旋转矩阵：
			Matrix temp_current_2 = { 0.0 };
			Matrix temp_father_2 = { 0.0 };

			temp_current_2.Index[0][0] = cos(bones[current_id].local_coord_axis.y);
			temp_current_2.Index[0][2] = sin(bones[current_id].local_coord_axis.y);
			temp_current_2.Index[1][1] = 1.0;
			temp_current_2.Index[2][0] = -sin(bones[current_id].local_coord_axis.y);
			temp_current_2.Index[2][2] = cos(bones[current_id].local_coord_axis.y);

			temp_father_2.Index[0][0] = cos(bones[father_id].local_coord_axis.y);
			temp_father_2.Index[0][2] = sin(bones[father_id].local_coord_axis.y);
			temp_father_2.Index[1][1] = 1.0;
			temp_father_2.Index[2][0] = -sin(bones[father_id].local_coord_axis.y);
			temp_father_2.Index[2][2] = cos(bones[father_id].local_coord_axis.y);

			///定义绕Z轴旋转矩阵：
			Matrix temp_current_3 = { 0.0 };
			Matrix temp_father_3 = { 0.0 };

			temp_current_3.Index[0][0] = cos(bones[current_id].local_coord_axis.z);
			temp_current_3.Index[0][1] = -sin(bones[current_id].local_coord_axis.z);
			temp_current_3.Index[1][0] = sin(bones[current_id].local_coord_axis.z);
			temp_current_3.Index[1][1] = cos(bones[current_id].local_coord_axis.z);
			temp_current_3.Index[2][2] = 1.0;

			temp_father_3.Index[0][0] = cos(bones[father_id].local_coord_axis.z);
			temp_father_3.Index[0][1] = -sin(bones[father_id].local_coord_axis.z);
			temp_father_3.Index[1][0] = sin(bones[father_id].local_coord_axis.z);
			temp_father_3.Index[1][1] = cos(bones[father_id].local_coord_axis.z);
			temp_father_3.Index[2][2] = 1.0;

			//==============旋转矩阵定义完毕，以下将其相乘构成组合旋转矩阵==============
			//这里的顺序仍然很重要

			Matrix rotate_matrix_current = MatrixMult(temp_current_2, temp_current_1);
			rotate_matrix_current = MatrixMult(temp_current_3, rotate_matrix_current);

			Matrix rotate_matrix_father = MatrixMult(temp_father_2, temp_father_1);
			rotate_matrix_father = MatrixMult(temp_father_3, rotate_matrix_father);

			//===========================组合旋转矩阵定义完毕。==========================
			//正式开始迁移：
			//迁移步骤一：
			temp = MatrixMultVec(rotate_matrix_current, temp);

			//迁移步骤二：
			//首先定义迁移平移向量，是基于原始局部坐标系下的。
			VECTOR temp_vec;
			temp_vec.x = bones[father_id].length*bones[father_id].direc.x;
			temp_vec.y = bones[father_id].length*bones[father_id].direc.y;
			temp_vec.z = bones[father_id].length*bones[father_id].direc.z;

			temp.x += temp_vec.x;
			temp.y += temp_vec.y;
			temp.z += temp_vec.z;

			//迁移步骤三：
			//这里就只用调用一次矩阵求逆，避免之前的四次调用，提高程序效率。
			Matrix temptemp = MatrixInverse(rotate_matrix_father);
			temp = MatrixMultVec(temptemp, temp);

			//以上，实现将当前骨骼的局部坐标迁移至其父骨骼的局部坐标系下。

			//最后更新临时变量：当前骨骼->父亲骨骼；父亲骨骼—>爷爷骨骼。
			current_id = father_id;
			father_id = bones[current_id].father_id;
		}
		//此时，current_id=0,即表示它的父亲是“根节点”了。此时已经将其迁移至根节点初始局部坐标系下的局部坐标了。

		//接下来完成最后一步，将其旋转、迁移至世界坐标系下，这一步是平凡的。
		//整个骨架的“旋转变换”，依旧遵循X-Y-Z的顺序进行。
		temp = Matrix_RotateX(bones[0].dof[0], temp);
		temp = Matrix_RotateY(bones[0].dof[1], temp);
		temp = Matrix_RotateZ(bones[0].dof[2], temp);
		//整个骨架的“平移变换”。
		temp.x += bones[0].global_coord.x;
		temp.y += bones[0].global_coord.y;
		temp.z += bones[0].global_coord.z;

		//至此，两种变换完毕，更新全局坐标
		bones[i].global_coord = temp;



	}
}
