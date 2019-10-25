#include "RawInput.h"

//定义回调函数
LRESULT CALLBACK WinSunProc(
	HWND hwnd,     //handle to window
	UINT uMsg,     //message identifier
	WPARAM wPARAM, //first message parameter
	LPARAM lParam //second message parameter
);

//程序入口函数
int WINAPI WinMain(
	HINSTANCE hInstance, //handle to current instance
	HINSTANCE hPrevInstance,//handle to previous instance
	LPSTR lpCmdLine,        //command line
	int nCmdShow            //show state
)
{
	//设计一个窗口类
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndcls.hInstance = hInstance;    //应用程序实例句柄由WinMain函数传进来
	wndcls.lpfnWndProc = WinSunProc; // 传入回调函数地址
	wndcls.lpszClassName = "mywindow";
	wndcls.lpszMenuName = NULL;
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndcls);        //注册窗口类
								   //创建窗口，定义一个变量用来保存成功创建窗口后返回的句柄
	HWND hwnd;
	hwnd = CreateWindow("mywindow", "my first application",
		WS_OVERLAPPEDWINDOW, 0, 0, 600, 400, NULL, NULL, hInstance, NULL);
	//显示及刷新窗口,SW_HIDE 设置为窗口隐藏
	ShowWindow(hwnd, SW_HIDE);
	UpdateWindow(hwnd);
	// 注册原始输入设备
	Init(hwnd);
	//定义消息结构体，开始消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg); //将虚拟键消息转换为字符消息
		DispatchMessage(&msg); //把消息传递给操作系统
	}
	return msg.wParam;
}
//编写窗口过程函数
LRESULT CALLBACK WinSunProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		Init(hwnd);
		break;
	case WM_INPUT:
		GetData(lParam);
		break;
	case WM_CHAR:           //在窗口中按下一个字符键
		break;
	case WM_LBUTTONDOWN:    //在窗口中按下鼠标左键时
		break;
	case WM_PAINT:         //重新绘制窗口
		break;
	case WM_CLOSE:          //关闭窗口时弹出消息框
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

