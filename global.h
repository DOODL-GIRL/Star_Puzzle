#pragma once
#include<map>
#include<vector>
#include<graphics.h>
using namespace std;

struct Snode
{

	int state;//直接用一个整型表示3*3的矩阵，节约存储空间
	int g;   //遍历深度g值
	int h;   //启发函数h值
	int num; //当前结点的编号（区分不同结点）
	int pre; //当前结点的前驱结点的编号（便于回溯）

	//对产生的新结构体的便捷赋初始值
	Snode(const int state, int g, int h, int num, int pre) :state(state), g(g), h(h), num(num), pre(pre) {};
	//比较运算符重载，便于open表的查重
	bool operator <(const Snode& e) const { return state < e.state; }
	bool operator ==(const Snode& e) const { return (state == e.state); }

};

typedef int Board[3][3];
static int move_x[] = { -1,0,1,0 };  //左移动是-1,右移动是+1
static int move_y[] = { 0,1,0,-1 };  //上移动是1,下移动是-1
static multimap<int, Snode> open_key;//open表
static map<Snode, int> open_value;   //open表的value-key反表，便于查找元素
static map <int, bool> closed;       //closed表
static vector <Snode> path;          //保存closed表的具体结构路径，便于回溯
static vector <Snode> whole_path;    //保存整个搜索过程的具体结构路径，便于生成搜索树

extern const int unit;//画布上每个数字的宽度
extern const int d;//初始状态与目标状态在画布上的距离
extern int expanded_num;//扩展结点数
extern int generated_num;//生成节点数