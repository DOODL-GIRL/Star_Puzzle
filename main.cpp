#include<iostream>
#include<map>
#include<vector>
#include<windows.h>
#include<graphics.h>
#include<conio.h>
#include "global.h"
#include "function.h"
using namespace std;


int main()
{
	/*��ʱ��ر���*/
	double time = 0;
	double counts = 0;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;

	/*����ʼĩ״̬��ѡ������ʽ����*/
	Board start, end;
	cout << "�������ʼ״̬��������ʽ���뼴�ɣ���" << endl;
	inPut(start);
	cout << "������Ŀ��״̬��������ʽ���뼴�ɣ���" << endl;
	inPut(end);
	int start_state;
	board_toState(start_state, start);
	int end_state;
	board_toState(end_state, end);
	int choice;
	cout << endl;
	cout << "=====================================================" << endl;
	cout << "��ѡ��Ҫʹ�õ�����ʽ������Ӧ��ţ�1/2)��             " << endl;
	cout << "=====================================================" << endl;
	cout << "1.�ò���λ��������Ϊ��������                         " << endl;
	cout << "2.���������ӵ�����Ŀ��λ�õ������پ�����Ϊ��������   " << endl;
	cout << "=====================================================" << endl;
	cin >> choice;

	/*��ʼ��������ʱ*/
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);//��ʼ��ʱ
	int step = run(start_state, end_state, choice);
	QueryPerformanceCounter(&nEndTime);//ֹͣ��ʱ

	/*��ʼ����չʾ*/
	initgraph(1000, 450 + 200);//��1000x450��С�Ļ���
	setbkcolor(WHITE);// ���ñ���ɫΪ��ɫ
	cleardevice();// �ñ���ɫ�����Ļ
	show_state(start_state, 0);
	show_state(end_state, 1);
	setbkmode(TRANSPARENT);
	LOGFONT f;
	gettextstyle(&f);// ��ȡ��ǰ��������
	f.lfHeight = 24;// ��������߶�
	_tcscpy_s(f.lfFaceName, _T("����"));// ��������
	f.lfQuality = ANTIALIASED_QUALITY;// �������Ч��Ϊ�����  
	settextstyle(&f);// ����������ʽ
	settextcolor(BLACK);// ����������ɫ
	TCHAR s[20];
	IMAGE img;
	loadimage(&img, _T("img\\start.jpg"), 200, 100, true);
	putimage(150, 500, &img);
	loadimage(&img, _T("img\\arrow.jpg"), 100, 50, true);
	putimage(450, 200, &img);
	MOUSEMSG m;		// ���������Ϣ
	while (true)
	{
		// ��ȡһ�������Ϣ
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONUP)
		{
			if (m.x >= 150 && m.x <= 350 && m.y >= 500 && m.y <= 600)
				break;
		}
	}

	/*���ӻ������ؽ�������·��������չ��㣬�����ɽ�㣩*/
	if (step != -1)
	{
		retrace(step);//��������·��
		loadimage(&img, _T("img\\blank.jpg"), 470, 220, true);
		putimage(0 + 3 * unit + d - 20, 480, &img);
		outtextxy(0 + 3 * unit + d, 500, _T("�н⣬���·���ǣ�����:"));
		_stprintf_s(s, _T("%d"), step);
		outtextxy(280 + 3 * unit + d, 500, s);
		outtextxy(0 + 3 * unit + d, 530, _T("����չ�������������"));
		_stprintf_s(s, _T("%d"), expanded_num);
		outtextxy(280 + 3 * unit + d, 530, s);
		outtextxy(0 + 3 * unit + d, 560, _T("�����ɽ������������"));
		_stprintf_s(s, _T("%d"), generated_num);
		outtextxy(280 + 3 * unit + d, 560, s);
		time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;//�������ִ��ʱ�䵥λΪs
		outtextxy(0 + 3 * unit + d, 590, _T("����ʱ�䣨���룩��"));
		int time_ms = time * 1000;
		_stprintf_s(s, _T("%d"), time_ms);
		outtextxy(280 + 3 * unit + d, 590, s);
		outtextxy(0 + 3 * unit + d, 620, _T("�����������ɣ�"));
	}
	else
	{
		loadimage(&img, _T("img\\no_way.jpg"), 100, 50, true);
		putimage(450, 200, &img);
		loadimage(&img, _T("img\\blank.jpg"), 470, 220, true);
		putimage(0 + 3 * unit + d - 20, 480, &img);
		outtextxy(0 + 3 * unit + d, 500, _T("�޽�"));
		outtextxy(0 + 3 * unit + d, 530, _T("δ������������"));
	}

	loadimage(&img, _T("img\\exit.jpg"), 200, 100, true);
	putimage(150, 500, &img);
	while (true)
	{
		// ��ȡһ�������Ϣ
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONUP)
		{
			if (m.x >= 150 && m.x <= 350 && m.y >= 500 && m.y <= 600)
				break;
		}
	}
	closegraph();//�رջ���
	if (step != -1)
	{
		cout << endl << "������������,���Ժ�......" << endl;
		create_tree();
		cout << "������������ɣ���˫������tree.bat���tree.png�鿴" << endl;
	}
	return 0;
}