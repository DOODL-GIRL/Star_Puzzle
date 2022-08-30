#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<graphics.h>
#include<fstream>
#include "global.h"
using namespace std;

int expanded_num = 0;//��չ�����
int generated_num = 0;//���ɽڵ���
const int unit = 150;//������ÿ�����ֵĿ��
const int d = 100;//��ʼ״̬��Ŀ��״̬�ڻ����ϵľ���

void state_toBoard(int state, Board arc)//��һά����ת��Ϊ��ά����
{
	for (int i = 2; i >= 0; i--)
	{
		for (int j = 2; j >= 0; j--)
		{
			arc[i][j] = state % 10;
			state = state / 10;
		}
	}
}

void board_toState(int& state, Board arc)//����ά����ת��Ϊһά����
{
	state = 0;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			state = state * 10 + arc[i][j];
	return;
}

int F(const Snode& node)//��С����·���Ĺ���ֵf(n)=g(n)+h(n)
{
	return node.g + node.h;
}

bool checkvalid(int cur, int tar)//��鵱ǰ״̬�Ƿ��ܵ���Ŀ��״̬��ͨ����������飩
{
	/*������״̬ת��Ϊ��ά����*/
	Board cur_arc;
	Board tar_arc;
	state_toBoard(cur, cur_arc);
	state_toBoard(tar, tar_arc);
	/*����ά����ת��Ϊ��ά����*/
	int a[9];
	int b[9];
	int k = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			a[k] = cur_arc[i][j];
			b[k] = tar_arc[i][j];
			k++;
		}
	}
	int a_count = 0;//����a����������������0��
	int b_count = 0;//����b����������������0��
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (a[i] != 0 && a[j] != 0 && a[j] > a[i]) a_count++;
			if (b[i] != 0 && b[j] != 0 && b[j] > b[i]) b_count++;
		}
	}
	/*��ǰ״̬��Ŀ��״̬��������ż����ͬ����ɴ�н⣩�����򲻿ɴ�޽⣩*/
	if (a_count % 2 == 0 && b_count % 2 == 0)return true;
	else if (a_count % 2 == 1 && b_count % 2 == 1)return true;
	else return false;
}

bool success(int cur, int tar)//�жϵ�ǰ״̬�Ƿ���Ŀ��״̬
{
	return cur == tar;
}

int H(int cur, int tar, int choice)//choiceΪ1��2�ֱ�������ֲ�ͬ������ʽ����
{
	if (choice == 1)//��һ������ʽ������h1=����λ������
	{
		int h = 0;//����λ���������ۼ�ֵ
		for (int i = 0; i < 9; i++)
		{
			if (cur % 10 != 0 && cur % 10 != tar % 10)
				h++;
			cur = cur / 10;
			tar = tar / 10;
		}
		return h;
	}
	else//�ڶ�������ʽ������h2=�������ӵ�����Ŀ��λ�õ������پ����
	{
		Board cur_arc;
		Board tar_arc;
		state_toBoard(cur, cur_arc);
		state_toBoard(tar, tar_arc);
		int cur_x, cur_y, tar_x, tar_y;
		int h = 0;
		for (int k = 1; k < 9; k++)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (cur_arc[i][j] == k)
					{  //��Ӧ��ʼ����±�
						cur_x = i;
						cur_y = j;
					}
					if (tar_arc[i][j] == k)
					{   //��ӦĿ��Ľ���±�
						tar_x = i;
						tar_y = j;
					}
				}
			}
			h += abs(cur_x - tar_x) + abs(cur_y - tar_y); //����h
		}
		return h;
	}
}

int run(const int st, const int ed, int choice) //����open�㷨������ֵȡ���·����stepֵ;choice����ѡ�����ֲ�ͬ������ʽ����
{
	extern int expanded_num;
	extern int generated_num;
	if (!checkvalid(st, ed)) //����Ƿ�ɴ���������������Ч��
		return -1;

	/*������һ�γ�������ʱ����������*/
	open_key.clear();
	open_value.clear();
	closed.clear();
	path.clear();
	whole_path.clear();

	/*����ʼ������open����*/
	int index = 0;//���������ֵ��Ψһ��ʶ�����ڲ�ѯpre·��

	Snode start(st, 0, H(st, ed, choice), index++, -1);
	open_key.insert(make_pair<int, Snode>(int(H(st, ed, choice)), Snode(start)));//����ʼ������open_key����
	open_value.insert(make_pair<Snode, int>(Snode(start), int(H(st, ed, choice))));//����ʼ������open_value����
	
	
	whole_path.push_back(start);//�����ľ���ṹ����path��
	
	
	generated_num++;

	/*�Ժ�������������ʽ����*/
	while (open_key.size())
	{
		Snode mixNode = open_key.begin()->second;//ȡ��open�ĵ�һ��Ԫ��(��Ԫ��Ҳ��fֵ��С�Ľ��)������չ
		open_key.erase(open_key.begin());                   //��open�������
		open_value.erase(open_value.lower_bound(mixNode));  //��open��ķ��������
		
		closed.insert(make_pair<int, bool>(int(mixNode.state), bool(true)));  //�����з���closed����
		path.push_back(mixNode);//�����ľ���ṹ����path��
		expanded_num++;

		//������Ѿ�����Ŀ��·�����������·��ֵstep
		if (success(mixNode.state, ed))
			return mixNode.g;

		/*��ȡ���Ľ������ƶ������������µ��ӽڵ㡣*/
		//Ѱ�ҿո��λ��
		int cx = -1, cy = -1;
		Board tmp_arc;
		state_toBoard(mixNode.state, tmp_arc);
		//cx��cy����ո������[cx,cy]
		for (int i = 0; i < 3; i++)
		{
			if (cx != -1) break;
			for (int j = 0; j < 3; j++)
			{
				if (tmp_arc[i][j] == 0)
				{
					cx = i;
					cy = j;
					break;
				}
			}
		}
		//�ƶ������ӽ��
		for (int k = 0; k < 4; k++)
		{
			//nx,ny�����ƶ���ո������[nx,ny]
			int nx = cx + move_x[k];
			int ny = cy + move_y[k];
			if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3)//��֤�ƶ���Ŀո�Խ��
			{
				swap(tmp_arc[cx][cy], tmp_arc[nx][ny]);//��ԭ���ո���ƶ���Ŀո�Ӧ�ڵ�λ�õ�Ԫ�أ�����˳��
				int tmp_state;
				board_toState(tmp_state, tmp_arc);  //��ȡ�������ɵ�״̬����stateֵ
				swap(tmp_arc[cx][cy], tmp_arc[nx][ny]);//��ԭ���У����ڿո�����������ƶ�

				/*�������ɵ��ӽڵ�����ж����Ƿ���open����*/
				Snode newNode(tmp_state, mixNode.g + 1, H(tmp_state, ed, choice), index++, mixNode.num);//��ʼ�������ɵ��ӽڵ�
				//�����ɵ��ӽڵ��state��tmp_state,����Ǹ��ڵ�mixNode�����+1,h���ж�����h��x)���,���кŰ���+1�����ڵ����к���mixNode�����к�num
				generated_num++;

				whole_path.push_back(newNode);//�����ľ���ṹ����path��


				int newF = F(newNode);//�����½���Fֵ
				//���������ɽ���state�Ƿ���closed����
				if (closed.count(tmp_state)) //�����closed���У�ֱ������
					continue;
				else//�������closed���У�ִ�����²���(map.count()����map�б�����Ԫ�صĸ�����
				{
					//���������ɽ���state�Ƿ���open����

					/*��Ϊopen�������Ԫ�ز�û��ֱ�ӱ�ʾstateֵ��ѡ�����count���������ã�����Ԫ�ص�ʱ�򣬾�Ҫ�Լ���дһ�������㷨
					/���մ�ͳ�Ĳ��ң���Ȼ�Ǳ���ÿ��Ԫ�أ���open�����Ƿ���Ԫ�ص�stateֵ���½���stateֵ��ͬ����ʱ������Ч�ʾ��е��������ֳ���˫map���������
					/˫map��key-value,value-key���ڲ���Ԫ�ص�ʱ�򣬲��ñ�����ֱ������low_bound()������value-key���еĵ�firstֵvalue
					/��value-key���У�firstֵvalue�Ĵ�С���������˳����������Snode�ıȽϴ�С�Ĺ��̣��ڶ����ʱ���Ѿ����壬��Snode a==Snode b,��Ȼ��������������ֵstateֵ��ͬ��<��Ҳ��stateֵ��Ƚϵ�
					*/
					map<Snode, int>::iterator it_v = open_value.lower_bound(newNode);
					//itvָ��ָ��stateֵ>=newNode�ĵ�����Ԫ�أ������Ԫ�ص�firstֵǡ��=newNode,˵��newNode������ֵstate��open���д��ڣ�����ǲ��ҵİ취

					map<int, Snode>::iterator it_k;//����ָ��key-value���������ָ��it_k
					if (it_v != open_value.end() && it_v->first == newNode) //�����open����
					{
						//��������ɵĽ���������ԭopen���н������Ž�
						if (newF < F(it_v->first))
						{
							for (it_k = open_key.lower_bound(F(it_v->first)); it_k != open_key.upper_bound(F(it_v->first)); ++it_k)
								if (it_k->second == newNode)
									break;
							//ɾ��ԭ��open���н��
							open_key.erase(it_k);
							open_value.erase(it_v);
							//���½�����open����
							open_key.insert(make_pair<int, Snode>(int(newF), Snode(newNode)));
							open_value.insert(make_pair<Snode, int>(Snode(newNode), int(newF)));
						}
						else  //�������Ž⣬���������ɵĽ��
							continue;
					}
					else //�Ȳ���open���У�Ҳ����closed����
					{
						//���½�����open����
						open_key.insert(make_pair<int, Snode>(int(newF), Snode(newNode)));
						open_value.insert(make_pair<Snode, int>(Snode(newNode), int(newF)));
					}
				}
			}
		}/*���ƶ����ɵ��½���������������*/
	}
	//�����ߵ��������open��Ϊ�գ����޽⣬���ز���-1
	return -1;
}

void inPut(Board s) //�����ʼ״̬
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cin >> s[i][j];
		}
	}
}

void show_block(int pos, int num, int choice)//��posλ�ô�չʾ����num,pos=0~8,num=0~8,choice=0��1�ֱ�����ڻ�����ߺ��ұ���ʾ
{
	int x = 0;
	int y = 0;
	IMAGE img;
	switch (pos)
	{
	case 0:
		x = 0, y = 0;
		break;
	case 1:
		x = unit, y = 0;
		break;
	case 2:
		x = 2 * unit, y = 0;
		break;
	case 3:
		x = 0, y = unit;
		break;
	case 4:
		x = unit, y = unit;
		break;
	case 5:
		x = 2 * unit, y = unit;
		break;
	case 6:
		x = 0, y = 2 * unit;
		break;
	case 7:
		x = unit, y = 2 * unit;
		break;
	case 8:
		x = 2 * unit, y = 2 * unit;
		break;
	default:
		break;
	}
	switch (num)
	{
	case 1:
		loadimage(&img, _T("img\\1.jpg"), unit, unit, true);//����ͼƬ
		break;
	case 2:
		loadimage(&img, _T("img\\2.jpg"), unit, unit, true);//����ͼƬ
		break;
	case 3:
		loadimage(&img, _T("img\\3.jpg"), unit, unit, true);//����ͼƬ
		break;
	case 4:
		loadimage(&img, _T("img\\4.jpg"), unit, unit, true);//����ͼƬ
		break;
	case 5:
		loadimage(&img, _T("img\\5.jpg"), unit, unit, true);//����ͼƬ
		break;
	case 6:
		loadimage(&img, _T("img\\6.jpg"), unit, unit, true);//����ͼƬ
		break;
	case 7:
		loadimage(&img, _T("img\\7.jpg"), unit, unit, true);//����ͼƬ
		break;
	case 8:
		loadimage(&img, _T("img\\8.jpg"), unit, unit, true);//����ͼƬ
		break;
	case 0:
	default:
		loadimage(&img, _T("img\\0.jpg"), unit, unit, true);//����ͼƬ
		break;
	}
	x = x + choice * (d + unit * 3);
	putimage(x, y, &img);  //��ʾͼƬ
	return;
}

void show_state(const int state, int choice)//������״̬���ӻ�չʾ����
{
	Board tmp;
	state_toBoard(state, tmp);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			show_block(i * 3 + j, tmp[i][j], choice);
		}
	}
}

void findPath(int pre, int size, int step) //�ݹ���·����size����δ���ݵĽڵ�������
{
	if (step == -1) //�޽�
	{
		return;
	}
	else if (step == 0)
	{
		show_state(path[size].state, 0);
		return;
	}
	for (int i = size; i >= 0; i--)
	{
		//�ҵ�path��ԭ����pre��㣬�ݹ���ú�����������
		if (path[i].num == pre)
			findPath(path[i].pre, i, step - 1);
	}
	show_state(path[size].state, 0);
	Sleep(100 * 3);
}

void retrace(int step)//��������·��
{
	//path.end()���ص���ĩβԪ������һ��Ԫ�صĵ�����
	findPath((path.end() - 1)->pre, path.size() - 1, step);
}

void create_tree()//�������ӻ�������
{
	ofstream out("tree.dot", ios::out|ios::binary);
	if (out.is_open() == NULL)
	{
		cout << "�����������ļ�ʧ�ܣ�" << endl;
		exit(EXIT_FAILURE);
	}
	out << "digraph {" << endl;
	out<<"node[shape = box]" << endl;
	for (int i = 0; i < whole_path.size(); i++)
	{
		out << whole_path[i].num << "[label=\"state\:\\n[";
		Board temp;
		state_toBoard(whole_path[i].state, temp);
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				out << temp[i][j] << " ";
			}
			if (i != 2)
				out << "\\n";
		}
		out << "]\\nf:" << F(whole_path[i]) << "\"];" << endl;
		
	}
	int key_num = (path.end() - 1)->num;
	for (int i = whole_path.size() - 1; i > 0; i--)
	{
		out << whole_path[i].pre << "->" << whole_path[i].num ;
		if (whole_path[i].num == key_num)
		{
			out << "[color=\"red\"]";
			key_num = whole_path[i].pre;
		}
		out << ";" << endl;
	}
	out << "}" << endl;
	out.close();
	return;
}

