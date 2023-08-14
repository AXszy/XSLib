/**
 * @file xappwindow.h 
 * @brief: 应用程序窗口类
 * @author: XSSunet
 * @date: 2023-07-09 10:32:18
 * 
 **/

#ifndef XAPPWINDOW_H
#define XAPPWINDOW_H

#include <vector>
#include "xswindow.h"
#include "xsgraphic.h"
#include "xsmovablepolygon.h"
#include "xspolygonbutton.h"
#include "xsanimatedpoint.h"
#include "xsmusic.h"
#include "xsscene.h"
#include "xsupdater.h"

class XAppWindow : public XSWindow {
public:
	XAppWindow();										/*构造函数*/
	XAppWindow(HINSTANCE hInst);						/*构造函数*/
	~XAppWindow();										/*析构函数*/
	int handle_message(HWND, UINT, WPARAM, LPARAM);		/*具体实现App功能的消息处理*/	
	void get_cursor_pos(HWND hwnd, XSPoint *p);			/*获得鼠标在窗口中的坐标*/
	
	void on_create();									/*创建窗口时的消息处理*/
private:
	//具体实现App功能的私有成员变量
	std::vector<XSScene *> m_list_scene;				/*场景列表*/
	XSScene *m_sc_init;									/*打开程序的第一个场景*/
};

#endif
