#pragma once
#include<map>
#include<vector>
#include "global.h"
using namespace std;

/*算法相关函数*/
void state_toBoard(int state, Board arc);//将一维数字转化为二维数组
void board_toState(int& state, Board arc);//将二维数组转换为一维数字
int F(const Snode& node);//最小代价路径的估计值f(n)=g(n)+h(n)
bool checkvalid(int cur, int tar);//检查当前状态是否能到达目标状态（通过逆序数检查）
bool success(int cur, int tar);//判断当前状态是否是目标状态
int H(int cur, int tar, int choice);//choice为1和2分别代表两种不同的启发式函数
int run(const int st, const int ed, int choice); //运行open算法，返回值取最短路径的step值;choice用来选择两种不同的启发式函数
void inPut(Board s); //输入初始状态
void findPath(int pre, int size, int step);//递归找路径
void retrace(int step);//回溯最短路径

/*可视化相关函数*/
void show_block(int pos, int num, int choice);//在pos位置处展示数字num,pos=0~8,num=0~8,choice=0和1分别代表在画布左边和右边显示
void show_state(const int state,int choice);//将棋子当前状态可视化展示出来
void create_tree();//创建可视化搜索树