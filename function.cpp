#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<graphics.h>
#include<fstream>
#include "global.h"
using namespace std;

int expanded_num = 0;//扩展结点数
int generated_num = 0;//生成节点数
const int unit = 150;//画布上每个数字的宽度
const int d = 100;//初始状态与目标状态在画布上的距离

void state_toBoard(int state, Board arc)//将一维数字转化为二维数组
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

void board_toState(int& state, Board arc)//将二维数组转换为一维数字
{
	state = 0;
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			state = state * 10 + arc[i][j];
	return;
}

int F(const Snode& node)//最小代价路径的估计值f(n)=g(n)+h(n)
{
	return node.g + node.h;
}

bool checkvalid(int cur, int tar)//检查当前状态是否能到达目标状态（通过逆序数检查）
{
	/*将整型状态转换为二维数组*/
	Board cur_arc;
	Board tar_arc;
	state_toBoard(cur, cur_arc);
	state_toBoard(tar, tar_arc);
	/*将二维数组转换为二维数组*/
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
	int a_count = 0;//数组a的逆序数（不包括0）
	int b_count = 0;//数组b的逆序数（不包括0）
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < i; j++)
		{
			if (a[i] != 0 && a[j] != 0 && a[j] > a[i]) a_count++;
			if (b[i] != 0 && b[j] != 0 && b[j] > b[i]) b_count++;
		}
	}
	/*当前状态与目标状态逆序数奇偶性相同，则可达（有解）；否则不可达（无解）*/
	if (a_count % 2 == 0 && b_count % 2 == 0)return true;
	else if (a_count % 2 == 1 && b_count % 2 == 1)return true;
	else return false;
}

bool success(int cur, int tar)//判断当前状态是否是目标状态
{
	return cur == tar;
}

int H(int cur, int tar, int choice)//choice为1和2分别代表两种不同的启发式函数
{
	if (choice == 1)//第一种启发式函数：h1=不在位棋子数
	{
		int h = 0;//不在位棋子总数累计值
		for (int i = 0; i < 9; i++)
		{
			if (cur % 10 != 0 && cur % 10 != tar % 10)
				h++;
			cur = cur / 10;
			tar = tar / 10;
		}
		return h;
	}
	else//第二种启发式函数：h2=所有棋子到达其目标位置的曼哈顿距离和
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
					{  //相应开始点的下标
						cur_x = i;
						cur_y = j;
					}
					if (tar_arc[i][j] == k)
					{   //相应目标的结点下标
						tar_x = i;
						tar_y = j;
					}
				}
			}
			h += abs(cur_x - tar_x) + abs(cur_y - tar_y); //计算h
		}
		return h;
	}
}

int run(const int st, const int ed, int choice) //运行open算法，返回值取最短路径的step值;choice用来选择两种不同的启发式函数
{
	extern int expanded_num;
	extern int generated_num;
	if (!checkvalid(st, ed)) //检测是否可达，这样可以提高运行效率
		return -1;

	/*清理上一次程序运行时产生的数据*/
	open_key.clear();
	open_value.clear();
	closed.clear();
	path.clear();
	whole_path.clear();

	/*将初始结点放入open表中*/
	int index = 0;//递增的序号值，唯一标识，便于查询pre路径

	Snode start(st, 0, H(st, ed, choice), index++, -1);
	open_key.insert(make_pair<int, Snode>(int(H(st, ed, choice)), Snode(start)));//将初始结点放入open_key表中
	open_value.insert(make_pair<Snode, int>(Snode(start), int(H(st, ed, choice))));//将初始结点放入open_value表中
	
	
	whole_path.push_back(start);//将结点的具体结构放入path中
	
	
	generated_num++;

	/*对后续结点进行启发式搜索*/
	while (open_key.size())
	{
		Snode mixNode = open_key.begin()->second;//取出open的第一个元素(该元素也是f值最小的结点)进行扩展
		open_key.erase(open_key.begin());                   //从open表中清除
		open_value.erase(open_value.lower_bound(mixNode));  //从open表的反表中清除
		
		closed.insert(make_pair<int, bool>(int(mixNode.state), bool(true)));  //将序列放入closed表中
		path.push_back(mixNode);//将结点的具体结构放入path中
		expanded_num++;

		//如果是已经到达目标路径，返回最短路径值step
		if (success(mixNode.state, ed))
			return mixNode.g;

		/*对取出的结点进行移动操作，生成新的子节点。*/
		//寻找空格的位置
		int cx = -1, cy = -1;
		Board tmp_arc;
		state_toBoard(mixNode.state, tmp_arc);
		//cx，cy代表空格的坐标[cx,cy]
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
		//移动生成子结点
		for (int k = 0; k < 4; k++)
		{
			//nx,ny代表移动后空格的坐标[nx,ny]
			int nx = cx + move_x[k];
			int ny = cy + move_y[k];
			if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3)//保证移动后的空格不越界
			{
				swap(tmp_arc[cx][cy], tmp_arc[nx][ny]);//将原来空格和移动后的空格应在的位置的元素，交换顺序
				int tmp_state;
				board_toState(tmp_state, tmp_arc);  //提取出新生成的状态序列state值
				swap(tmp_arc[cx][cy], tmp_arc[nx][ny]);//还原序列，用于空格其他方向的移动

				/*对新生成的子节点进行判定，是否入open表中*/
				Snode newNode(tmp_state, mixNode.g + 1, H(tmp_state, ed, choice), index++, mixNode.num);//初始化新生成的子节点
				//新生成的子节点的state是tmp_state,深度是父节点mixNode的深度+1,h用判定函数h（x)求得,序列号按序+1，父节点序列号是mixNode的序列号num
				generated_num++;

				whole_path.push_back(newNode);//将结点的具体结构放入path中


				int newF = F(newNode);//保存新结点的F值
				//查找新生成结点的state是否在closed表中
				if (closed.count(tmp_state)) //如果在closed表中，直接跳过
					continue;
				else//如果不在closed表中，执行以下操作(map.count()返回map中被查找元素的个数）
				{
					//查找新生成结点的state是否在open表中

					/*因为open表的两个元素并没有直接表示state值的选项，所以count函数不能用，查找元素的时候，就要自己重写一个查找算法
					/按照传统的查找，必然是遍历每个元素，看open表中是否有元素的state值跟新结点的state值相同，这时候搜索效率就有点大，这就体现出来双map表的优势了
					/双map表，key-value,value-key，在查找元素的时候，不用遍历，直接利用low_bound()函数找value-key表中的的first值value
					/在value-key表中，first值value的从小到大的排列顺序是新类型Snode的比较大小的过程，在定义的时候已经定义，若Snode a==Snode b,必然是它们俩的序列值state值相同，<号也是state值相比较的
					*/
					map<Snode, int>::iterator it_v = open_value.lower_bound(newNode);
					//itv指针指向state值>=newNode的迭代器元素，如果该元素的first值恰好=newNode,说明newNode的序列值state在open表中存在，这就是查找的办法

					map<int, Snode>::iterator it_k;//创建指向key-value表迭代器的指针it_k
					if (it_v != open_value.end() && it_v->first == newNode) //如果在open表中
					{
						//如果新生成的结点是相对于原open表中结点的最优解
						if (newF < F(it_v->first))
						{
							for (it_k = open_key.lower_bound(F(it_v->first)); it_k != open_key.upper_bound(F(it_v->first)); ++it_k)
								if (it_k->second == newNode)
									break;
							//删除原有open表中结点
							open_key.erase(it_k);
							open_value.erase(it_v);
							//将新结点加入open表中
							open_key.insert(make_pair<int, Snode>(int(newF), Snode(newNode)));
							open_value.insert(make_pair<Snode, int>(Snode(newNode), int(newF)));
						}
						else  //不是最优解，放弃新生成的结点
							continue;
					}
					else //既不在open表中，也不在closed表中
					{
						//将新结点加入open表中
						open_key.insert(make_pair<int, Snode>(int(newF), Snode(newNode)));
						open_value.insert(make_pair<Snode, int>(Snode(newNode), int(newF)));
					}
				}
			}
		}/*对移动生成的新结点的整个操作结束*/
	}
	//函数走到这里，代表open表为空，则无解，返回步骤-1
	return -1;
}

void inPut(Board s) //输入初始状态
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cin >> s[i][j];
		}
	}
}

void show_block(int pos, int num, int choice)//在pos位置处展示数字num,pos=0~8,num=0~8,choice=0和1分别代表在画布左边和右边显示
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
		loadimage(&img, _T("img\\1.jpg"), unit, unit, true);//加载图片
		break;
	case 2:
		loadimage(&img, _T("img\\2.jpg"), unit, unit, true);//加载图片
		break;
	case 3:
		loadimage(&img, _T("img\\3.jpg"), unit, unit, true);//加载图片
		break;
	case 4:
		loadimage(&img, _T("img\\4.jpg"), unit, unit, true);//加载图片
		break;
	case 5:
		loadimage(&img, _T("img\\5.jpg"), unit, unit, true);//加载图片
		break;
	case 6:
		loadimage(&img, _T("img\\6.jpg"), unit, unit, true);//加载图片
		break;
	case 7:
		loadimage(&img, _T("img\\7.jpg"), unit, unit, true);//加载图片
		break;
	case 8:
		loadimage(&img, _T("img\\8.jpg"), unit, unit, true);//加载图片
		break;
	case 0:
	default:
		loadimage(&img, _T("img\\0.jpg"), unit, unit, true);//加载图片
		break;
	}
	x = x + choice * (d + unit * 3);
	putimage(x, y, &img);  //显示图片
	return;
}

void show_state(const int state, int choice)//将棋子状态可视化展示出来
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

void findPath(int pre, int size, int step) //递归找路径（size是尚未回溯的节点数量）
{
	if (step == -1) //无解
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
		//找到path中原结点的pre结点，递归调用函数，输出结果
		if (path[i].num == pre)
			findPath(path[i].pre, i, step - 1);
	}
	show_state(path[size].state, 0);
	Sleep(100 * 3);
}

void retrace(int step)//回溯所有路径
{
	//path.end()返回的是末尾元素再下一个元素的迭代器
	findPath((path.end() - 1)->pre, path.size() - 1, step);
}

void create_tree()//创建可视化搜索树
{
	ofstream out("tree.dot", ios::out|ios::binary);
	if (out.is_open() == NULL)
	{
		cout << "搜索树生成文件失败！" << endl;
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

