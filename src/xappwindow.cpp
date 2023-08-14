/**
 * @file xappwindow.cpp
 * @brief: 应用程序窗口的实现
 * @author: XSSunet
 * @date: 2023-07-09 10:32:35
 *
 **/

#include "xappwindow.h"
#include "xsgraphic.h"
#include "xsmovablepolygon.h"
#include "xspolygonbutton.h"
#include "xsanimatedpoint.h"
#include "xsmusic.h"
#include "xsscene.h"
#include "xsconfig.h"
#include <math.h>
#include <cstdio>

#define XS_MAIN_TIMER 1

XAppWindow::XAppWindow() : XSWindow() {

}

XAppWindow::XAppWindow(HINSTANCE hInst) : XSWindow(hInst) {

}

int XAppWindow::handle_message(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp) {

	switch (wm) {
		case WM_CREATE: {
			this->on_create();
			SetTimer(hwnd, XS_MAIN_TIMER, 2, NULL);
			break;
		}
		case WM_PAINT: {
			XSGraphic *g = new XSGraphic(hwnd);
			
			for(XSScene *i : this->m_list_scene)
			{
				i->on_paint(g);
			}
			
			delete g;
			break;
		}
		case WM_MOUSEMOVE: {
			XSPoint p_cur;
			this->get_cursor_pos(this->getHWND(), &p_cur);

			for(XSScene *i : this->m_list_scene)
			{
				i->on_mousemove(p_cur);
			}
			
			break;
		}
		case WM_LBUTTONDOWN: {
			XSPoint p_cur;
			this->get_cursor_pos(this->getHWND(), &p_cur);

			for(XSScene *i : this->m_list_scene)
			{
				i->on_lbuttondown(p_cur);
			}
			
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		case WM_LBUTTONUP: {
			XSPoint p_cur;
			this->get_cursor_pos(this->getHWND(), &p_cur);

			for(XSScene *i : this->m_list_scene)
			{
				i->on_lbuttonup(p_cur);
			}
			
			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}
		case WM_TIMER: {
			switch (wp) {
				case XS_MAIN_TIMER: {
					InvalidateRect(hwnd, NULL, FALSE);
					XSPoint p_cur;
					this->get_cursor_pos(this->getHWND(), &p_cur);

					for(XSScene *i : this->m_list_scene)
					{
						i->on_timer(p_cur);
					}
					
					break;
				}
			}
			break;
		}
		case WM_KEYDOWN: {
			for(XSScene *i : this->m_list_scene)
			{
				i->on_keydown(wp);
			}
			
			break;
		}
		case WM_KEYUP: {
			for(XSScene *i : this->m_list_scene)
			{
				i->on_keydown(wp);
			}
			
			break;
		}
		default: {
			return 0;
			break;
		}
	}
	return 1;
}

void XAppWindow::get_cursor_pos(HWND hwnd, XSPoint *p) {
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);

	//获得客户区宽度和高度
	RECT rcClient;
	GetClientRect(hwnd, &rcClient);
	int client_w = rcClient.right - rcClient.left;
	int client_h = rcClient.bottom - rcClient.top;

	//计算实际坐标
	if (client_w > 0 && client_h > 0) {
		p->x = pt.x * 1.0 / client_w;
		p->y = pt.y * 1.0 / client_h;
	} else {
		p->x = p->y = 0;
	}
}

void XAppWindow::on_create() {
	
	//初始化场景
	{
		this->m_sc_init = new XSScene();
		this->m_list_scene.push_back(m_sc_init);
	}
	this->m_sc_init->use();
}

XAppWindow::~XAppWindow() {
	//释放资源
	for(auto i : this->m_list_scene) delete i;
	
	//释放GDI资源
	
	this->release();
}
