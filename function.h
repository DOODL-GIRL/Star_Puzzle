#pragma once
#include<map>
#include<vector>
#include "global.h"
using namespace std;

/*�㷨��غ���*/
void state_toBoard(int state, Board arc);//��һά����ת��Ϊ��ά����
void board_toState(int& state, Board arc);//����ά����ת��Ϊһά����
int F(const Snode& node);//��С����·���Ĺ���ֵf(n)=g(n)+h(n)
bool checkvalid(int cur, int tar);//��鵱ǰ״̬�Ƿ��ܵ���Ŀ��״̬��ͨ����������飩
bool success(int cur, int tar);//�жϵ�ǰ״̬�Ƿ���Ŀ��״̬
int H(int cur, int tar, int choice);//choiceΪ1��2�ֱ�������ֲ�ͬ������ʽ����
int run(const int st, const int ed, int choice); //����open�㷨������ֵȡ���·����stepֵ;choice����ѡ�����ֲ�ͬ������ʽ����
void inPut(Board s); //�����ʼ״̬
void findPath(int pre, int size, int step);//�ݹ���·��
void retrace(int step);//�������·��

/*���ӻ���غ���*/
void show_block(int pos, int num, int choice);//��posλ�ô�չʾ����num,pos=0~8,num=0~8,choice=0��1�ֱ�����ڻ�����ߺ��ұ���ʾ
void show_state(const int state,int choice);//�����ӵ�ǰ״̬���ӻ�չʾ����
void create_tree();//�������ӻ�������