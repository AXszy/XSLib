/**
 * @file xswindow.h 
 * @brief: 窗口类
 * @author: XSSunet
 * @date: 2023-07-09 10:31:46
 * 
 **/

#ifndef XSWINDOW_H
#define XSWINDOW_H

#include <windows.h>
#include <vector>
#include <string>

class XSWindow {
public:
	XSWindow();		/*构造函数*/
	XSWindow(HINSTANCE hInst);		/*构造函数*/
	virtual ~XSWindow();	/*析构函数*/
	
	void create();		/*创建窗口*/
	HWND getHWND(); 	/*获得窗口句柄*/
	virtual int handle_message(HWND, UINT, WPARAM, LPARAM) = 0; 	/*消息处理*/
	void release(); 	/*内存释放*/
private:
	HWND m_hwnd; 		/*窗口句柄*/
	HINSTANCE m_hInst;	/*应用程序实例句柄*/
	static std::vector<void*> registeredClassList; 	/*注册过的类表*/
	static CALLBACK LRESULT WndProc(HWND, UINT, WPARAM, LPARAM);  /*窗口回调函数*/
};

#endif
