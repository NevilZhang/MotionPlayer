#include "Motion.h"
Motion::Motion() {
	total_frame_num = 0;	//��֡��
}

void Motion::Read_ASF(string filename)
{
	ifstream asfData(filename);
	if (!asfData) {
		cerr << "�ļ���ʧ��" << endl;
	}
	string	line = "0";
	string	key_words = "0";		//���ڱ���ؼ��֣��ж�״̬
	string	tag = "0";			    //tag���Ա�����������״̬
	//���ȴ�����ڵ㣬��ʼ��
	skeletons[0].bones[0].id = 0;
	skeletons[0].bones[0].name = "root";
	skeletons[0].bones[0].length = 0.0;
	skeletons[0].name_to_id["root"] = 0;
	//���ڵ㴦�����

	//ѭ��������ļ����ж���
	while (getline(asfData,line)){
		stringstream ss(line);
		ss >> key_words;

		//��������״̬���
		if (key_words == ":root")		tag = ":root";
		if (key_words == ":bonedata")	tag = ":bonedata";
		if (key_words == ":hierarchy")	tag = ":hierarchy";

		//:root״̬��
		if (key_words == "position" && tag == ":root") {	//��ȡ���ڵ�����
			ss >> skeletons[0].bones[0].global_coord.x;
			ss >> skeletons[0].bones[0].global_coord.y;
			ss >> skeletons[0].bones[0].global_coord.z;
		}
		if (key_words == "orientation" && tag == ":root"){	//Ŀǰ�������˸��ڵ��ʼ�ֲ�����ϵ
			ss >> skeletons[0].bones[0].local_coord_axis.x;
			ss >> skeletons[0].bones[0].local_coord_axis.y;
			ss >> skeletons[0].bones[0].local_coord_axis.z;
			//ASF�����ļ��е������ǽǶȣ�����Ӧ��תΪ���ȣ����������������
			skeletons[0].bones[0].local_coord_axis.x = skeletons[0].bones[0].local_coord_axis.x / 180.0*PI;
			skeletons[0].bones[0].local_coord_axis.y = skeletons[0].bones[0].local_coord_axis.y / 180.0*PI;
			skeletons[0].bones[0].local_coord_axis.z = skeletons[0].bones[0].local_coord_axis.z / 180.0*PI;
		}

		//:bonedata״̬��
		if (key_words == "begin" && tag == ":bonedata") {
			skeletons[0].bone_num++;
		}

		if (key_words == "id" && tag == ":bonedata") {
			ss >> skeletons[0].bones[skeletons[0].bone_num].id;
			if (skeletons[0].bone_num != skeletons[0].bones[skeletons[0].bone_num].id)
				cerr << "������ȡ��������" << endl;
		}
		if (key_words == "name" && tag == ":bonedata") {
			ss >> skeletons[0].bones[skeletons[0].bone_num].name;
			skeletons[0].name_to_id[skeletons[0].bones[skeletons[0].bone_num].name] = 
				skeletons[0].bones[skeletons[0].bone_num].id;	//ͨ��map�󶨹�����ID
		}
		if (key_words == "direction" && tag == ":bonedata") { 
			//��ȡ������ʼdirection
			ss >> skeletons[0].bones[skeletons[0].bone_num].direc.x;
			ss >> skeletons[0].bones[skeletons[0].bone_num].direc.y;
			ss >> skeletons[0].bones[skeletons[0].bone_num].direc.z;
		}
		if (key_words == "length" && tag == ":bonedata") { 
			//��ȡ����length
			ss >> skeletons[0].bones[skeletons[0].bone_num].length;
		}
		if (key_words == "axis" && tag == ":bonedata") {
			//��ȡ����axis�ֲ�������
			ss >> skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.x;
			ss >> skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.y;
			ss >> skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.z;
			//ASF�����ļ��е������ǽǶȣ�����Ӧ��תΪ���ȣ����������������
			skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.x =
				skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.x / 180.0*PI;
			skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.y =
				skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.y / 180.0*PI;
			skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.z =
				skeletons[0].bones[skeletons[0].bone_num].local_coord_axis.z / 180.0*PI;
		}
		if (key_words == "dof" && tag == ":bonedata")        //��ȡ����dof�������ɶ�
		{
			string temp = "0";
			while (ss >> temp)
			{
				if (temp == "rx")
					skeletons[0].bones[skeletons[0].bone_num].dofflag[0] = 1;		//�൱�ڴ򿪿���
				if (temp == "ry")
					skeletons[0].bones[skeletons[0].bone_num].dofflag[1] = 1;		//�൱�ڴ򿪿���
				if (temp == "rz")
					skeletons[0].bones[skeletons[0].bone_num].dofflag[2] = 1;		//�൱�ڴ򿪿���
			}
		}

		//:hierarchy״̬��
		if (key_words != ":hierarchy" && key_words != "begin" && tag == ":hierarchy") {
			if (key_words == "end") {
				break;	//��ȡ���
			}
			string temp;
			while (ss>>temp) {
				//�������ڵ��Ϊ�ؼ��ֽڵ���ӽڵ�
				skeletons[0].bones[skeletons[0].name_to_id[key_words]].child_id.push_back(skeletons[0].name_to_id[temp]);
				//���ؼ��ֽڵ���ӵ������ڵ�ĸ��ڵ�
				skeletons[0].bones[skeletons[0].name_to_id[temp]].father_id = skeletons[0].name_to_id[key_words];		
			}
		}
	}
	//cout << "end" << endl;
	//���ر���
	asfData.close();
}

void Motion::Read_AMC(string filename) {
	//��������
	ifstream amcData(filename);
	if (!amcData) {
		cerr << "AMC�ļ���ʧ��" << endl;
	}
	
	int curr_frame_num = 0;		//���Ա�������ȡ����������֡����Ҫ��main����ļ������֡�
								//Ҳ��Ϊ֡״̬ʶ���
	int total_frame_num = 0;	//ÿ�ζ�ȡ����ɽ��֡������˶��л���֡���ٵ��˶������������ǰ�˶������⡣
	string line = "0";

	//ѭ���壬���������ļ������ж���
	while (getline(amcData, line)) {
		//��ȡһ�еĵ�һ������
		string key_words = "0";
		stringstream ss(line);
		ss >> key_words;

		//��������һ�У���ͬ�ȶԴ������Թؼ�����Ҫ����һ���ж�:
		//��������֣���ǰ֡״̬����Ϊ�����֣�ֱ��������һ������״̬�Żᷢ���ı�
		
		//isdigit��ʾ��ǰ������������
		if (isdigit(key_words[0])) {
			istringstream temp_string;		//��ʱ�ַ�������ת��
			temp_string.str(key_words);
			temp_string >> curr_frame_num;	//���µ�ǰ֡��
			
			skeletons[curr_frame_num] = skeletons[0];	//��skeletons[0]�Ǽ���Ϣ�������ǰ�Ǽ�֡����Ϊ����֡�õ���ͬһ���Ǽ�

			//�������֡��
			if (curr_frame_num > total_frame_num) {
				total_frame_num = curr_frame_num;
			}
		}
		else {
			//�������ɶ����ݣ����������Ƹ��ڵ�ֲ�����ϵ��ת�ĽǶ�
			//��Ҫ�Ѹ��ڵ㵥�����������������ڵ��ȫ������Ƚ����⣬ֱ�����ˣ�������㡣
			if (key_words == "root") {
				ss >> skeletons[curr_frame_num].bones[0].global_coord.x;
				ss >> skeletons[curr_frame_num].bones[0].global_coord.y;
				ss >> skeletons[curr_frame_num].bones[0].global_coord.z;

				ss >> skeletons[curr_frame_num].bones[0].dof[0];
				ss >> skeletons[curr_frame_num].bones[0].dof[1];
				ss >> skeletons[curr_frame_num].bones[0].dof[2];

				//AMC�����ļ��е������ǽǶȣ�����Ӧ��תΪ���ȣ����������������
				skeletons[curr_frame_num].bones[0].dof[0] = skeletons[curr_frame_num].bones[0].dof[0] / 180.0*PI;
				skeletons[curr_frame_num].bones[0].dof[1] = skeletons[curr_frame_num].bones[0].dof[1] / 180.0*PI;
				skeletons[curr_frame_num].bones[0].dof[2] = skeletons[curr_frame_num].bones[0].dof[2] / 180.0*PI;
			}
			//����������Ǹ��ڵ�
			else {
				double temp = 0.0;
				while (ss >> temp) {
					for (int i = 0; i < 3; i++)    //˳������ȥ��ֻҪ���ؿ��ŵĻ�����Ȼ������ǰ�rxryrz˳��ͻ���bug
					{
						if (skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dofflag[i] == 1){
							skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dof[i] = temp;						
							//AMC�����ļ��е������ǽǶȣ�����Ӧ��תΪ���ȣ����������������
							skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dof[i] =
								skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dof[i] / 180.0*PI;
							//���عرա�
							skeletons[curr_frame_num].bones[skeletons[curr_frame_num].name_to_id[key_words]].dofflag[i] = 0;	
							break;
						}
					}
				}
			}



		}
	}

}

//�������ÿһ֡�Ǽ�������Ϣ�ĺ�����
void Motion::cal_position()
{
	for (int i = 1; i <= total_frame_num; i++)
	{
		skeletons[i].cal_global_coord();
	}
}