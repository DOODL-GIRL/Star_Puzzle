#pragma once
#include<map>
#include<vector>
#include<graphics.h>
using namespace std;

struct Snode
{

	int state;//ֱ����һ�����ͱ�ʾ3*3�ľ��󣬽�Լ�洢�ռ�
	int g;   //�������gֵ
	int h;   //��������hֵ
	int num; //��ǰ���ı�ţ����ֲ�ͬ��㣩
	int pre; //��ǰ����ǰ�����ı�ţ����ڻ��ݣ�

	//�Բ������½ṹ��ı�ݸ���ʼֵ
	Snode(const int state, int g, int h, int num, int pre) :state(state), g(g), h(h), num(num), pre(pre) {};
	//�Ƚ���������أ�����open��Ĳ���
	bool operator <(const Snode& e) const { return state < e.state; }
	bool operator ==(const Snode& e) const { return (state == e.state); }

};

typedef int Board[3][3];
static int move_x[] = { -1,0,1,0 };  //���ƶ���-1,���ƶ���+1
static int move_y[] = { 0,1,0,-1 };  //���ƶ���1,���ƶ���-1
static multimap<int, Snode> open_key;//open��
static map<Snode, int> open_value;   //open���value-key�������ڲ���Ԫ��
static map <int, bool> closed;       //closed��
static vector <Snode> path;          //����closed��ľ���ṹ·�������ڻ���
static vector <Snode> whole_path;    //���������������̵ľ���ṹ·������������������

extern const int unit;//������ÿ�����ֵĿ��
extern const int d;//��ʼ״̬��Ŀ��״̬�ڻ����ϵľ���
extern int expanded_num;//��չ�����
extern int generated_num;//���ɽڵ���