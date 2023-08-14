/**
 * @file xswindow.cpp 
 * @brief: 窗口类的实现
 * @author: XSSunet
 * @date: 2023-07-09 10:32:00
 * 
 **/

#include "xswindow.h"

std::vector<void*> XSWindow::registeredClassList;

XSWindow::XSWindow()
{
	this->m_hwnd = nullptr;
	this->m_hInst = nullptr;
}

XSWindow::XSWindow(HINSTANCE hInst)
{
	this->m_hwnd = nullptr;
	this->m_hInst = hInst;
}

XSWindow::~XSWindow()
{
	this->release();
}

void XSWindow::create()
{
	TCHAR szClassName[32];
	TCHAR szTitleName[128];
	void *_vPtr = *((void**)this);
	::wsprintf(szClassName, TEXT("%p"), _vPtr);
	
	//查表检查是否注册过
	std::vector<void*>::iterator it;
	for(it = registeredClassList.begin(); it != registeredClassList.end(); it++)
	{
		if((*it) == _vPtr)
		{
			break;
		}
	}
	//未注册则进行注册
	if(it == registeredClassList.end())
	{
		//注册窗口
		WNDCLASSEX wc;
		wc.cbClsExtra = 0;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.cbWndExtra = 0;
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = wc.hIcon;
		wc.lpfnWndProc = XSWindow::WndProc;
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		wc.hInstance = this->m_hInst;
		wc.lpszClassName = szClassName;
		wc.lpszMenuName = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		
		if(::RegisterClassEx(&wc))
		{
			registeredClassList.push_back(_vPtr);
		}
	}
	
	//创建窗口
	if(!this->m_hwnd)
	{
		::wsprintf(szTitleName, TEXT("%p"), _vPtr);
		HWND hwnd = CreateWindow(
			szClassName,
			szTitleName,
			WS_OVERLAPPEDWINDOW,
			0, 0, 800, 600, 
			NULL,
			NULL,
			this->m_hInst,
			(LPVOID)this 	/*将指针保存到CreateParams*/
			);
		
		if(!hwnd)
		{
			this->m_hwnd = NULL;
			MessageBox(NULL, TEXT("创建窗口失败!"), TEXT("错误"), MB_OK);
			return;
		}
	}
}

HWND XSWindow::getHWND()
{
	return this->m_hwnd;
}

CALLBACK LRESULT XSWindow::WndProc(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp)
{
	XSWindow *pObj = (XSWindow*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch(wm)
	{
	case WM_CREATE:
		{
			//把窗口对象句柄赋给C++对象成员
			pObj = (XSWindow*)(((LPCREATESTRUCT)lp)->lpCreateParams);
			pObj->m_hwnd = hwnd;
			//把C++对象地址赋给窗口对象成员 
			::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONGLONG)pObj);
			pObj->handle_message(hwnd, wm, wp, lp);
			break;
		}
	case WM_DESTROY:
		{
			if(pObj)
			{
				pObj->m_hwnd = NULL;
			}
			break;
		}
	default:
		{
			pObj = (XSWindow*)(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if(pObj)
			{
				if(!pObj->handle_message(hwnd, wm, wp, lp))
				{
					return DefWindowProc(hwnd, wm, wp, lp);
				}
			}
			else
			{
				return DefWindowProc(hwnd, wm, wp, lp);
			}
			break;
		}
	}
	return 0;
}

void XSWindow::release()
{
	// C++对象被销毁之前，销毁窗口对象
	if ( this->m_hwnd && ::IsWindow(this->m_hwnd) )   
	{  
		::DestroyWindow(this->m_hwnd);
	}
}
