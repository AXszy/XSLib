/**
 * @file xsgraphic.cpp 
 * @brief: 绘图类的实现
 * @author: XSSunet
 * @date: 2023-07-09 10:33:04
 * 
 **/

#include "xsgraphic.h"
#include <windows.h>
#include <cmath>

XSGraphic::XSGraphic(HWND hwnd)
{
	this->m_hwnd = hwnd;
	this->m_hdc = ::BeginPaint(this->m_hwnd, &this->m_ps);
	
	//获得客户区宽度和高度
	RECT rcClient;
	GetClientRect(m_hwnd, &rcClient);
	this->m_width = rcClient.right - rcClient.left;
	this->m_height = rcClient.bottom - rcClient.top;
	
	//双缓冲绘图
	this->m_combmp = ::CreateCompatibleBitmap(
		this->m_hdc, 
		XSGRAPHIC_COMPATIBLEBMP_W, 
		XSGRAPHIC_COMPATIBLEBMP_H
		);
	this->m_comdc = ::CreateCompatibleDC(this->m_hdc);
	::SelectObject(this->m_comdc, this->m_combmp);
	
	::SetStretchBltMode(this->m_comdc, COLORONCOLOR);
	
	this->m_comcomdc = ::CreateCompatibleDC(this->m_comdc);
}

XSGraphic::~XSGraphic()
{
	//将兼容DC的内容拷贝到DC上
	::StretchBlt(
		this->m_hdc, 
		0, 0, this->m_width, this->m_height, 
		this->m_comdc, 
		0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H,
		SRCCOPY
		);
	
	::DeleteDC(this->m_comdc);
	::DeleteDC(this->m_comcomdc);
	::DeleteObject(this->m_combmp);
	::EndPaint(this->m_hwnd, &this->m_ps);
}

void XSGraphic::select(HGDIOBJ ho)
{
	::SelectObject(this->m_comdc, ho);
}

void XSGraphic::rectangle_bypixel(int l, int t, int r, int b)
{
	::Rectangle(this->m_comdc, l, t, r, b);
}

void XSGraphic::ellipse_bypixel(int l, int t, int r, int b)
{
	::Ellipse(this->m_comdc, l, t, r, b);
}

void XSGraphic::arc_bypixel(int l, int t, int r, int b, int x1, int y1, int x2, int y2)
{
	::Arc(this->m_comdc, l, t, r, b, x1, y1, x2,y2);
}

void XSGraphic::polyline_bypixel(POINT *apt, int cpt)
{
	::Polyline(this->m_comdc, apt, cpt);
}

void XSGraphic::polygon_bypixel(POINT *apt, int cpt)
{
	::Polygon(this->m_comdc, apt, cpt);
}

void XSGraphic::poly_bezier_bypixel(POINT *apt, int cpt)
{
	::PolyBezier(this->m_comdc, apt, cpt);
}

void XSGraphic::gradient_fill_bypixel(TRIVERTEX *pvertex, int nvertex, void *pmesh, int nmesh, long mode)
{
	::GradientFill(this->m_comdc, pvertex, nvertex, pmesh, nmesh, mode);
}

void XSGraphic::draw_image_bypixel(HBITMAP hbm, int x, int y, int w, int h)
{
	::SelectObject(this->m_comcomdc, hbm);
	
	BITMAP bm;
	::GetObject(hbm, sizeof(bm), &bm);
	
	StretchBlt(this->m_comdc, x, y, w, h, this->m_comcomdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
}

void XSGraphic::draw_image_bypixel(HBITMAP hbm, HBITMAP hbmMask, int x, int y, int w, int h)
{
	BITMAP bm;
	
	::SelectObject(this->m_comcomdc, hbmMask);
	::GetObject(hbmMask, sizeof(bm), &bm);
	StretchBlt(this->m_comdc, x, y, w, h, this->m_comcomdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCPAINT);
	
	::SelectObject(this->m_comcomdc, hbm);
	::GetObject(hbm, sizeof(bm), &bm);
	StretchBlt(this->m_comdc, x, y, w, h, this->m_comcomdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCAND);
}

void XSGraphic::draw_bits_bypixel(void *lpbits, int bmw, int bmh, int x, int y, int w, int h, DWORD mode)
{
	BITMAPINFO binfo;
	
	ZeroMemory(&binfo,sizeof(BITMAPINFO));
	binfo.bmiHeader.biBitCount=24;      //每个像素多少位，也可直接写24(RGB)或者32(RGBA)
	binfo.bmiHeader.biCompression=0;
	binfo.bmiHeader.biHeight=bmh;
	binfo.bmiHeader.biPlanes=1;
	binfo.bmiHeader.biSizeImage=0;
	binfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	binfo.bmiHeader.biWidth=bmw;
	
	//SetDIBits(this->m_comdc,this->m_combmp,0,binfo.bmiHeader.biHeight,lpbits,(BITMAPINFO*)&binfo,DIB_RGB_COLORS);
	StretchDIBits(this->m_comdc, x, y, w, h, 0, 0, bmw, bmh - 1, lpbits, &binfo, DIB_RGB_COLORS, mode);
}

void XSGraphic::plg_blt_bypixel(HBITMAP hbm, COLORREF crmask, POINT &lt, POINT &rt, POINT &ld)
{
	POINT apt[3] = {lt, rt, ld};
	BITMAP bm;
	
	HDC mdc = CreateCompatibleDC(m_comdc);
	::SelectObject(mdc, hbm);
	::GetObject(hbm, sizeof(bm), &bm);
	
	HBITMAP mbm = CreateCompatibleBitmap(this->m_comdc, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H);
	RECT rc = {0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H};
	HBRUSH hbr = CreateSolidBrush(crmask);
	SelectObject(this->m_comcomdc, mbm);
	FillRect(m_comcomdc, &rc, hbr);
	
	PlgBlt(this->m_comcomdc, apt, mdc, 0, 0, bm.bmWidth, bm.bmHeight, 0, 0, 0);
	TransparentBlt(this->m_comdc, 0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H, 
		this->m_comcomdc, 0, 0, XSGRAPHIC_COMPATIBLEBMP_W, XSGRAPHIC_COMPATIBLEBMP_H, crmask);
	
	DeleteDC(mdc);
	DeleteObject(hbr);
	DeleteObject(mbm);
}

void XSGraphic::draw_text_bypixel(int l, int t, int r, int b, TCHAR *str, DWORD align)
{
	RECT rc = {l, t, r, b};
	::DrawTextEx(this->m_comdc, str, lstrlen(str), &rc, align, NULL);
}

void XSGraphic::draw_image_part_bypixel(HBITMAP hbm, double ksrcx, double ksrcy, double ksrcw, double ksrch, int dstx, int dsty, int dstw, int dsth)
{
	::SelectObject(this->m_comcomdc, hbm);
	
	BITMAP bm;
	::GetObject(hbm, sizeof(bm), &bm);
	
	StretchBlt(this->m_comdc, dstx, dsty, dstw, dsth, 
		this->m_comcomdc, bm.bmWidth * ksrcx, bm.bmHeight * ksrcy, 
		bm.bmWidth * ksrcw, bm.bmHeight * ksrch, SRCCOPY);
}

void XSGraphic::draw_image_part_bypixel(HBITMAP hbm, HBITMAP hbmMask, double ksrcx, double ksrcy, double ksrcw, double ksrch, int dstx, int dsty, int dstw, int dsth)
{
	BITMAP bm;
	
	::SelectObject(this->m_comcomdc, hbmMask);
	::GetObject(hbmMask, sizeof(bm), &bm);
	StretchBlt(this->m_comdc, dstx, dsty, dstw, dsth, 
		this->m_comcomdc, bm.bmWidth * ksrcx, bm.bmHeight * ksrcy, 
		bm.bmWidth * ksrcw, bm.bmHeight * ksrch, SRCPAINT);
	
	::SelectObject(this->m_comcomdc, hbm);
	::GetObject(hbm, sizeof(bm), &bm);
	StretchBlt(this->m_comdc, dstx, dsty, dstw, dsth, 
		this->m_comcomdc, bm.bmWidth * ksrcx, bm.bmHeight * ksrcy, 
		bm.bmWidth * ksrcw, bm.bmHeight * ksrch, SRCAND);
}

void XSGraphic::rectangle(double l, double t, double r, double b)
{
	this->rectangle_bypixel(l * XSGRAPHIC_COMPATIBLEBMP_W , t * XSGRAPHIC_COMPATIBLEBMP_H, 
		r * XSGRAPHIC_COMPATIBLEBMP_W, b * XSGRAPHIC_COMPATIBLEBMP_H);
}

void XSGraphic::ellipse(double l, double t, double r, double b)
{
	this->ellipse_bypixel(l * XSGRAPHIC_COMPATIBLEBMP_W , t * XSGRAPHIC_COMPATIBLEBMP_H, 
		r * XSGRAPHIC_COMPATIBLEBMP_W, b * XSGRAPHIC_COMPATIBLEBMP_H);
}

void XSGraphic::arc(double l, double t, double r, double b, double x1, double y1, double x2, double y2)
{
	this->arc_bypixel(l * XSGRAPHIC_COMPATIBLEBMP_W , t * XSGRAPHIC_COMPATIBLEBMP_H, 
		r * XSGRAPHIC_COMPATIBLEBMP_W, b * XSGRAPHIC_COMPATIBLEBMP_H, 
		x1 * XSGRAPHIC_COMPATIBLEBMP_W, y1 * XSGRAPHIC_COMPATIBLEBMP_H,
		x2 * XSGRAPHIC_COMPATIBLEBMP_W, y2 * XSGRAPHIC_COMPATIBLEBMP_H);
}

void XSGraphic::polyline(XSPoint *apt, int cpt)
{
	POINT *pts = new POINT[cpt];
	for(int i = 0; i < cpt; i++)
	{
		pts[i].x = apt[i].x * XSGRAPHIC_COMPATIBLEBMP_W;
		pts[i].y = apt[i].y * XSGRAPHIC_COMPATIBLEBMP_H;
	}
	this->polyline_bypixel(pts, cpt);
	delete[] pts;
}

void XSGraphic::polygon(XSPoint *apt, int cpt)
{
	POINT *pts = new POINT[cpt];
	for(int i = 0; i < cpt; i++)
	{
		pts[i].x = apt[i].x * XSGRAPHIC_COMPATIBLEBMP_W;
		pts[i].y = apt[i].y * XSGRAPHIC_COMPATIBLEBMP_H;
	}
	this->polygon_bypixel(pts, cpt);
	delete[] pts;
}

void XSGraphic::poly_bezier(XSPoint *apt, int cpt)
{
	POINT *pts = new POINT[cpt];
	for(int i = 0; i < cpt; i++)
	{
		pts[i].x = apt[i].x * XSGRAPHIC_COMPATIBLEBMP_W;
		pts[i].y = apt[i].y * XSGRAPHIC_COMPATIBLEBMP_H;
	}
	this->poly_bezier_bypixel(pts, cpt);
	delete[] pts;
}

void XSGraphic::gradient_rect(XSPoint lt, COLORREF col1, XSPoint rb, COLORREF col2, long mode)
{
	TRIVERTEX vtx[2];
	vtx[0].x = lt.x * XSGRAPHIC_COMPATIBLEBMP_W;
	vtx[0].y = lt.y * XSGRAPHIC_COMPATIBLEBMP_H;
	vtx[0].Alpha = 0;
	vtx[0].Red = GetRValue(col1) * 0xffffff / 255;
	vtx[0].Green = GetGValue(col1) * 0xffffff / 255;
	vtx[0].Blue = GetBValue(col1) * 0xffffff / 255;
	
	vtx[1].x = rb.x * XSGRAPHIC_COMPATIBLEBMP_W;
	vtx[1].y = rb.y * XSGRAPHIC_COMPATIBLEBMP_H;
	vtx[1].Alpha = 0;
	vtx[1].Red = GetRValue(col2) * 0xffffff / 255;
	vtx[1].Green = GetGValue(col2) * 0xffffff / 255;
	vtx[1].Blue = GetBValue(col2) * 0xffffff / 255;
	
	GRADIENT_RECT rc[1];
	rc[0].UpperLeft = 0;
	rc[0].LowerRight = 1;
	
	this->gradient_fill_bypixel(vtx, 2, rc, 1, mode);
}

void XSGraphic::gradient_triangle(XSPoint p1, XSPoint p2, XSPoint p3, COLORREF c1, COLORREF c2, COLORREF c3)
{
	TRIVERTEX vtx[3];
	vtx[0].x = p1.x * XSGRAPHIC_COMPATIBLEBMP_W;
	vtx[0].y = p1.y * XSGRAPHIC_COMPATIBLEBMP_H;
	vtx[0].Alpha = 0;
	vtx[0].Red = GetRValue(c1) * 0xffffff / 255;
	vtx[0].Green = GetGValue(c1) * 0xffffff / 255;
	vtx[0].Blue = GetBValue(c1) * 0xffffff / 255;
	
	vtx[1].x = p2.x * XSGRAPHIC_COMPATIBLEBMP_W;
	vtx[1].y = p2.y * XSGRAPHIC_COMPATIBLEBMP_H;
	vtx[1].Alpha = 0;
	vtx[1].Red = GetRValue(c2) * 0xffffff / 255;
	vtx[1].Green = GetGValue(c2) * 0xffffff / 255;
	vtx[1].Blue = GetBValue(c2) * 0xffffff / 255;
	
	vtx[2].x = p3.x * XSGRAPHIC_COMPATIBLEBMP_W;
	vtx[2].y = p3.y * XSGRAPHIC_COMPATIBLEBMP_H;
	vtx[2].Alpha = 0;
	vtx[2].Red = GetRValue(c3) * 0xffffff / 255;
	vtx[2].Green = GetGValue(c3) * 0xffffff / 255;
	vtx[2].Blue = GetBValue(c3) * 0xffffff / 255;
	
	GRADIENT_TRIANGLE tri[1];
	tri[0].Vertex1 = 0;
	tri[0].Vertex2 = 1;
	tri[0].Vertex3 = 2;
	
	this->gradient_fill_bypixel(vtx, 3, tri, 1, GRADIENT_FILL_TRIANGLE);
}

void XSGraphic::plg_blt(HBITMAP hbm, COLORREF crmask, XSPoint &lt, XSPoint &rt, XSPoint &ld)
{
	POINT p_lt, p_rt, p_ld;
	p_lt.x = lt.x * XSGRAPHIC_COMPATIBLEBMP_W;
	p_lt.y = lt.y * XSGRAPHIC_COMPATIBLEBMP_H;
	p_rt.x = rt.x * XSGRAPHIC_COMPATIBLEBMP_W;
	p_rt.y = rt.y * XSGRAPHIC_COMPATIBLEBMP_H;
	p_ld.x = ld.x * XSGRAPHIC_COMPATIBLEBMP_W;
	p_ld.y = ld.y * XSGRAPHIC_COMPATIBLEBMP_H;
	this->plg_blt_bypixel(hbm, crmask, p_lt, p_rt, p_ld);
}

void XSGraphic::draw_text(double l, double t, double r, double b, TCHAR *str, DWORD align)
{
	this->draw_text_bypixel(l * XSGRAPHIC_COMPATIBLEBMP_W , t * XSGRAPHIC_COMPATIBLEBMP_H, 
		r * XSGRAPHIC_COMPATIBLEBMP_W, b * XSGRAPHIC_COMPATIBLEBMP_H,
		str, align);
}

void XSGraphic::draw_image(HBITMAP hbm, double x, double y, double w, double h)
{
	this->draw_image_bypixel(hbm, x * XSGRAPHIC_COMPATIBLEBMP_W, 
		y * XSGRAPHIC_COMPATIBLEBMP_H,
		w * XSGRAPHIC_COMPATIBLEBMP_W,
		h * XSGRAPHIC_COMPATIBLEBMP_H);
}

void XSGraphic::draw_image(HBITMAP hbm, HBITMAP hbmMask, double x, double y, double w, double h)
{
	this->draw_image_bypixel(hbm, hbmMask, x * XSGRAPHIC_COMPATIBLEBMP_W, 
		y * XSGRAPHIC_COMPATIBLEBMP_H,
		w * XSGRAPHIC_COMPATIBLEBMP_W,
		h * XSGRAPHIC_COMPATIBLEBMP_H);
}

void XSGraphic::draw_image_part(HBITMAP hbm, double ksrcx, double ksrcy, double ksrcw, double ksrch, double dstx, double dsty, double dstw, double dsth)
{
	this->draw_image_part_bypixel(hbm, ksrcx, ksrcy, ksrcw, ksrch, 
		dstx * XSGRAPHIC_COMPATIBLEBMP_W, dsty * XSGRAPHIC_COMPATIBLEBMP_H,
		dstw * XSGRAPHIC_COMPATIBLEBMP_W, dsth * XSGRAPHIC_COMPATIBLEBMP_H);
}

void XSGraphic::draw_image_part(HBITMAP hbm, HBITMAP hbmMask, double ksrcx, double ksrcy, double ksrcw, double ksrch, int dstx, int dsty, int dstw, int dsth)
{
	this->draw_image_part_bypixel(hbm, hbmMask, ksrcx, ksrcy, ksrcw, ksrch, 
		dstx * XSGRAPHIC_COMPATIBLEBMP_W, dsty * XSGRAPHIC_COMPATIBLEBMP_H,
		dstw * XSGRAPHIC_COMPATIBLEBMP_W, dsth * XSGRAPHIC_COMPATIBLEBMP_H);
}

void XSGraphic::draw_bits(void *lpbits, int bmw, int bmh, double x, double y, double w, double h, DWORD mode)
{
	this->draw_bits_bypixel(lpbits, bmw, bmh, x * XSGRAPHIC_COMPATIBLEBMP_W, 
		y * XSGRAPHIC_COMPATIBLEBMP_H, 
		w * XSGRAPHIC_COMPATIBLEBMP_W,
		h * XSGRAPHIC_COMPATIBLEBMP_H, mode);
}

