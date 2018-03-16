#include "Motion.h"
Motion::Motion() {
	total_frame_num = 0;	//总帧数
}

void Motion::Read_ASF(string filename)
{
	ifstream asfData(filename);
	if (!asfData) {
		cerr << "文件打开失败" << endl;
	}
	string	line = "0";
	string	key_words = "0";		//用于保存关键字，判断状态
	string	tag = "0";			    //tag用以标明该行所属状态
	//首先处理根节点，初始化
	skeletons[0].bones[0].id = 0;
	skeletons[0].bones[0].name = "root";
	skeletons[0].bones[0].length = 0.0;
	skeletons[0].name_to_id["root"] = 0;
	//根节点处理完毕

	//循环体控制文件逐行读入
	while (getline(asfData,line)){
		stringstream ss(line);
		ss >> key_words;

		//以下设置状态标记
		if (key_words == ":root")		tag = ":root";
		if (key_words == ":bonedata")	tag = ":bonedata";
		if (key_words == ":hierarchy")	tag = ":hierarchy";

		//:root状态下
		if (key_words == "position" && tag == ":root") {	//读取根节点坐标
			ss >> skeletons[0].bones[0].global_coord.x;
			ss >> skeletons[0].bones[0].global_coord.y;
			ss >> skeletons[0].bones[0].global_coord.z;
		}
		if (key_words == "orientation" && tag == ":root"){	//目前，读到了根节点初始局部坐标系
			ss >> skeletons[0].bones[0].local_coord_axis.x;
			ss >> skeletons[0].bones[0].local_coord_axis.y;
			ss >> skeletons[0].bones[0].local_coord_axis.z;
			//ASF数据文件中的数据是角度，这里应该转为弧度，否则后续计算会出错
			skeletons[0].bones[0].local_coord_axis.x = skeletons[0].bones[0].local_coord_axis.x / 180.0*PI;
			skeletons[0].bones[0].local_coord_axis.y = skeletons[0].bones[0].local_coord_axis.y / 180.0*PI;
			skeletons[0].bones[0].local_coord_axis.z = skeletons[0].bones[0].local_coord_axis.z / 180.0*PI;
		}

		//:bonedata状态下
		if (key_words == "begin" && tag == ":bonedata") {
			skeletons[0].bone_num++;
		}

		if (key_words == "id" && tag == ":bonedata") {
			ss >> skeletons[0].bones[skeletons[0].bone_num].id;
			if (skeletons[0].bone_num != skeletons[0].bones[skeletons[0].bone_num].id)
				cerr << "骨骼读取发生错误！" << endl;
		}
		if (key_words == "name" && tag == ":bonedata") {
			ss >> skeletons[0].bones[skeletons[0].bone_num].name;
			skeletons[0].name_to_id[skeletons[0].bones[skeletons[0].bone_num].name] = 
				skeletons[0].bones[skeletons[0].bone_num].id;	//通过map绑定骨骼与ID
		}
		if (key_words == "direction" && tag == ":bonedata") { 
			//读取骨骼初始direction
			ss >> skeletons[0].bones[skeletons[0].bone_num].direc.x;
			ss >> skeletons[0].bones[skeletons[0].bone_num].direc.y;
			ss >> skeletons[0].bones[skeletons[0].bone_num].direc.z;
		}
		if (key_words == "length" && tag == ":bonedata") { 
			//读取骨骼length
			ss >> skeletons[0].bones[skeletons[0].bone_num].length;
		}
		if (key_words == "axis" && tag == ":bonedata") {
			//读取骨骼axis局部坐标轴
			ss >> skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.x;
			ss >> skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.y;
			ss >> skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.z;
			//ASF数据文件中的数据是角度，这里应该转为弧度，否则后续计算会出错。
			skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.x =
				skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.x / 180.0*PI;
			skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.y =
				skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.y / 180.0*PI;
			skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.z =
				skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.z / 180.0*PI;
		}
		if (key_words == "dof" && tag == ":bonedata")        //读取骨骼dof，即自由度
		{
			string temp = "0";
			while (ss >> temp)
			{
				if (temp == "rx")
					skeletons[0].bones[skeletons[0].bone_num].dofflag[0] = 1;		//相当于打开开关
				if (temp == "ry")
					skeletons[0].bones[skeletons[0].bone_num].dofflag[1] = 1;		//相当于打开开关
				if (temp == "rz")
					skeletons[0].bones[skeletons[0].bone_num].dofflag[2] = 1;		//相当于打开开关
			}
		}

		//:hierarchy状态下
		if (key_words != ":hierarchy" && key_words != "begin" && tag == ":hierarchy") {
			if (key_words == "end") {
				break;	//读取完成
			}
			string temp;
			while (ss>>temp) {
				//将后续节点加为关键字节点的子节点
				skeletons[0].bones[skeletons[0].name_to_id[key_words]].child_id.push_back(skeletons[0].name_to_id[temp]);
				//将关键字节点添加到后续节点的父节点
				skeletons[0].bones[skeletons[0].name_to_id[temp]].father_id = skeletons[0].name_to_id[key_words];		
			}
		}
	}
	//cout << "end" << endl;
	//最后关闭流
	asfData.close();
}

void Motion::Read_AMC(string filename) {
	//打开输入流
	ifstream amcData(filename);
	if (!amcData) {
		cerr << "AMC文件打开失败" << endl;
	}
	
	int curr_frame_num = 0;		//用以保存所读取的行所处的帧数，要和main里面的加以区分。
								//也作为帧状态识别符
	int total_frame_num = 0;	//每次读取归零可解决帧数多的运动切换到帧数少的运动后继续播放先前运动的问题。
	string line = "0";

	//循环体，控制整个文件的逐行读入
	while (getline(amcData, line)) {
		//读取一行的第一个单词
		string key_words = "0";
		stringstream ss(line);
		ss >> key_words;

		//对于任意一行，均同等对待，但对关键字需要作出一个判断:
		//如果是数字，当前帧状态设置为该数字，直到遇到下一个数字状态才会发生改变
		
		//isdigit表示当前读入行是数字
		if (isdigit(key_words[0])) {
			istringstream temp_string;		//临时字符串辅助转化
			temp_string.str(key_words);
			temp_string >> curr_frame_num;	//更新当前帧数
			
			skeletons[curr_frame_num] = skeletons[0];	//把skeletons[0]骨架信息分享给当前骨架帧，因为所有帧用的是同一个骨架

			//更新最大帧数
			if (curr_frame_num > total_frame_num) {
				total_frame_num = curr_frame_num;
			}
		}
		else {
			//加载自由度数据，即各骨骼绕父节点局部坐标系旋转的角度
			//需要把根节点单独拎出来处理，这里根节点的全局坐标比较特殊，直接有了，无需计算。
			if (key_words == "root") {
				ss >> skeletons[curr_frame_num].bones[0].global_coord.x;
				ss >> skeletons[curr_frame_num].bones[0].global_coord.y;
				ss >> skeletons[curr_frame_num].bones[0].global_coord.z;

				ss >> skeletons[curr_frame_num].bones[0].dof[0];
				ss >> skeletons[curr_frame_num].bones[0].dof[1];
				ss >> skeletons[curr_frame_num].bones[0].dof[2];

				//AMC数据文件中的数据是角度，这里应该转为弧度，否则后续计算会出错。
				skeletons[curr_frame_num].bones[0].dof[0] = skeletons[curr_frame_num].bones[0].dof[0] / 180.0*PI;
				skeletons[curr_frame_num].bones[0].dof[1] = skeletons[curr_frame_num].bones[0].dof[1] / 180.0*PI;
				skeletons[curr_frame_num].bones[0].dof[2] = skeletons[curr_frame_num].bones[0].dof[2] / 180.0*PI;
			}
			//接下来处理非根节点
			else {
				double temp = 0.0;
				while (ss >> temp) {
					for (int i = 0; i < 3; i++)    //顺着塞进去，只要开关开着的话。当然如果不是按rxryrz顺序就会有bug
					{
						if (skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dofflag[i] == 1){
							skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dof[i] = temp;						
							//AMC数据文件中的数据是角度，这里应该转为弧度，否则后续计算会出错。
							skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dof[i] =
								skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dof[i] / 180.0*PI;
							//开关关闭。
							skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dofflag[i] = 0;	
							break;
						}
					}
				}
			}



		}
	}

}

//定义计算每一帧骨架坐标信息的函数。
void Motion::cal_position()
{
	for (int i = 1; i <= total_frame_num; i++)
	{
		skeletons[i].cal_global_coord();
	}
}