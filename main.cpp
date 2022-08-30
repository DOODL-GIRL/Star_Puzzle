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
	/*计时相关变量*/
	double time = 0;
	double counts = 0;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;

	/*输入始末状态，选择启发式函数*/
	Board start, end;
	cout << "请输入初始状态（矩阵形式输入即可）：" << endl;
	inPut(start);
	cout << "请输入目标状态（矩阵形式输入即可）：" << endl;
	inPut(end);
	int start_state;
	board_toState(start_state, start);
	int end_state;
	board_toState(end_state, end);
	int choice;
	cout << endl;
	cout << "=====================================================" << endl;
	cout << "请选择要使用的启发式函数对应序号（1/2)：             " << endl;
	cout << "=====================================================" << endl;
	cout << "1.用不在位棋子数作为启发函数                         " << endl;
	cout << "2.用所有棋子到达其目标位置的曼哈顿距离作为启发函数   " << endl;
	cout << "=====================================================" << endl;
	cin >> choice;

	/*开始搜索并计时*/
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime);//开始计时
	int step = run(start_state, end_state, choice);
	QueryPerformanceCounter(&nEndTime);//停止计时

	/*初始界面展示*/
	initgraph(1000, 450 + 200);//打开1000x450大小的画布
	setbkcolor(WHITE);// 设置背景色为白色
	cleardevice();// 用背景色清空屏幕
	show_state(start_state, 0);
	show_state(end_state, 1);
	setbkmode(TRANSPARENT);
	LOGFONT f;
	gettextstyle(&f);// 获取当前字体设置
	f.lfHeight = 24;// 设置字体高度
	_tcscpy_s(f.lfFaceName, _T("黑体"));// 设置字体
	f.lfQuality = ANTIALIASED_QUALITY;// 设置输出效果为抗锯齿  
	settextstyle(&f);// 设置字体样式
	settextcolor(BLACK);// 设置字体颜色
	TCHAR s[20];
	IMAGE img;
	loadimage(&img, _T("img\\start.jpg"), 200, 100, true);
	putimage(150, 500, &img);
	loadimage(&img, _T("img\\arrow.jpg"), 100, 50, true);
	putimage(450, 200, &img);
	MOUSEMSG m;		// 定义鼠标消息
	while (true)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONUP)
		{
			if (m.x >= 150 && m.x <= 350 && m.y >= 500 && m.y <= 600)
				break;
		}
	}

	/*可视化输出相关结果（最短路径，已扩展结点，已生成结点）*/
	if (step != -1)
	{
		retrace(step);//回溯整个路径
		loadimage(&img, _T("img\\blank.jpg"), 470, 220, true);
		putimage(0 + 3 * unit + d - 20, 480, &img);
		outtextxy(0 + 3 * unit + d, 500, _T("有解，最短路径是（步）:"));
		_stprintf_s(s, _T("%d"), step);
		outtextxy(280 + 3 * unit + d, 500, s);
		outtextxy(0 + 3 * unit + d, 530, _T("已扩展结点数（个）："));
		_stprintf_s(s, _T("%d"), expanded_num);
		outtextxy(280 + 3 * unit + d, 530, s);
		outtextxy(0 + 3 * unit + d, 560, _T("已生成结点数（个）："));
		_stprintf_s(s, _T("%d"), generated_num);
		outtextxy(280 + 3 * unit + d, 560, s);
		time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;//计算程序执行时间单位为s
		outtextxy(0 + 3 * unit + d, 590, _T("运行时间（毫秒）："));
		int time_ms = time * 1000;
		_stprintf_s(s, _T("%d"), time_ms);
		outtextxy(280 + 3 * unit + d, 590, s);
		outtextxy(0 + 3 * unit + d, 620, _T("搜索树已生成！"));
	}
	else
	{
		loadimage(&img, _T("img\\no_way.jpg"), 100, 50, true);
		putimage(450, 200, &img);
		loadimage(&img, _T("img\\blank.jpg"), 470, 220, true);
		putimage(0 + 3 * unit + d - 20, 480, &img);
		outtextxy(0 + 3 * unit + d, 500, _T("无解"));
		outtextxy(0 + 3 * unit + d, 530, _T("未生成搜索树！"));
	}

	loadimage(&img, _T("img\\exit.jpg"), 200, 100, true);
	putimage(150, 500, &img);
	while (true)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONUP)
		{
			if (m.x >= 150 && m.x <= 350 && m.y >= 500 && m.y <= 600)
				break;
		}
	}
	closegraph();//关闭画布
	if (step != -1)
	{
		cout << endl << "搜索树生成中,请稍候......" << endl;
		create_tree();
		cout << "搜索树生成完成！请双击运行tree.bat后打开tree.png查看" << endl;
	}
	return 0;
}