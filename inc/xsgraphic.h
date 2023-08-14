/**
 * @file xsgraphic.h 
 * @brief: 绘图类
 * @author: XSSunet
 * @date: 2023-07-09 10:32:51
 * 
 **/

#ifndef XSGRAPHIC_H
#define XSGRAPHIC_H

#include <windows.h>

struct XSPoint
{
	double x;
	double y;
};

class XSGraphic {
public:
	XSGraphic() = delete;		/*删除缺省的构造函数*/
	XSGraphic(HWND hwnd);		/*构造函数*/
	~XSGraphic();		/*析构函数*/
	
	void select(HGDIOBJ ho); 		/*选择GDI对象 : 画笔, 画刷...*/
	
	void rectangle(double l, double t, double r, double b);		/*绘制矩形*/
	void ellipse(double l, double t, double r, double b);		/*绘制椭圆*/
	void arc(double l, double t, double r, double b, double x1, double y1, double x2, double y2);	/*绘制椭圆弧*/
	void polyline(XSPoint *apt, int cpt);		/*绘制折线*/
	void polygon(XSPoint *apt, int cpt);		/*绘制多边形*/
	void poly_bezier(XSPoint *apt, int cpt); 	/*绘制三次 bezier 曲线*/
	void gradient_rect(XSPoint lt, COLORREF col1, XSPoint rb, COLORREF col2, long mode);	/*渐变矩形*/
	void gradient_triangle(XSPoint p1, XSPoint p2, XSPoint p3, COLORREF c1, COLORREF c2, COLORREF c3);	/*渐变三角*/
	void plg_blt(HBITMAP hbm, COLORREF crmask, XSPoint &lt, XSPoint &rt, XSPoint &ld);	/*平行四边形映射*/
	
	void draw_text(double l, double t, double r, double b, TCHAR *str, DWORD align);	/*将文字输出到屏幕上某矩形框*/
	
	void draw_image(HBITMAP hbm, double x, double y, double w, double h);		/*绘制位图*/
	void draw_image(HBITMAP hbm, HBITMAP hbmMask, double x, double y, double w, double h);	/*绘制透明绘图*/
	void draw_image_part(HBITMAP hbm, double ksrcx, double ksrcy, double ksrcw, double ksrch, double dstx, double dsty, double dstw, double dsth);		/*绘制位图的一部分到指定位置*/
	void draw_image_part(HBITMAP hbm, HBITMAP hbmMask, double ksrcx, double ksrcy, double ksrcw, double ksrch, int dstx, int dsty, int dstw, int dsth);		/*将透明位图一部分绘制到指定位置*/
	void draw_bits(void *lpbits, int bmw, int bmh, double x, double y, double w, double h, DWORD mode = SRCCOPY); /*绘制像素点*/
	
	void rectangle_bypixel(int l, int t, int r, int b);		/*绘制矩形*/
	void ellipse_bypixel(int l, int t, int r, int b);		/*绘制椭圆*/
	void arc_bypixel(int l, int t, int r, int b, int x1, int y1, int x2, int y2);	/*绘制椭圆弧*/
	void polyline_bypixel(POINT *apt, int cpt);		/*绘制折线*/
	void polygon_bypixel(POINT *apt, int cpt);		/*绘制多边形*/
	void poly_bezier_bypixel(POINT *apt, int cpt); 	/*绘制三次 bezier 曲线*/
	void gradient_fill_bypixel(TRIVERTEX *pvertex, int nvertex, void *pmesh, int nmesh, long mode);	/*渐变绘图*/
	void plg_blt_bypixel(HBITMAP hbm, COLORREF crmask, POINT &lt, POINT &rt, POINT &ld);	/*平行四边形映射*/
	
	void draw_text_bypixel(int l, int t, int r, int b, TCHAR *str, DWORD align);	/*将文字输出到屏幕上某矩形框*/
	
	void draw_image_bypixel(HBITMAP hbm, int x, int y, int w, int h);		/*绘制位图*/
	void draw_image_bypixel(HBITMAP hbm, HBITMAP hbmMask, int x, int y, int w, int h);	/*绘制透明绘图*/
	void draw_image_part_bypixel(HBITMAP hbm, double ksrcx, double ksrcy, double ksrcw, double ksrch, int dstx, int dsty, int dstw, int dsth);		/*绘制位图的一部分到指定位置*/
	void draw_image_part_bypixel(HBITMAP hbm, HBITMAP hbmMask, double ksrcx, double ksrcy, double ksrcw, double ksrch, int dstx, int dsty, int dstw, int dsth);		/*将透明位图一部分绘制到指定位置*/
	void draw_bits_bypixel(void *lpbits, int bmw, int bmh, int x, int y, int w, int h, DWORD mode = SRCCOPY); /*绘制像素点*/
private:
	HWND m_hwnd;	/*窗口句柄*/
	PAINTSTRUCT m_ps;		/*绘图结构体*/
	HDC m_hdc; 	/*设备上下文句柄*/
	HBITMAP m_combmp;		/*双缓冲 : 内存兼容位图 尺寸: XSGRAPHIC_COMPATIBLEBMP_W * XSGRAPHIC_COMPATIBLEBMP_H*/
	HDC m_comdc;			/*双缓冲 : 内存兼容设备上下文*/
	HDC m_comcomdc;
	int m_width; 		/*绘图区宽度*/
	int m_height;		/*绘图区高度*/
};

#define XSGRAPHIC_COMPATIBLEBMP_W		780
#define XSGRAPHIC_COMPATIBLEBMP_H		480

#endif
