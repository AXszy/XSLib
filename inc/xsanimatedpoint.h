/**
 * @file xsanimatedpoint.h 
 * @brief: 循环动画的点
 * @author: XSSunet
 * @date: 2023-07-30 13:24:18
 * 
 **/
#ifndef XSANIMATEDPOINT_H
#define XSANIMATEDPOINT_H

#include "xsanimate.h"
#include "xsgraphic.h"
#include <functional>

class XSAnimatedPoint : public XSAnimate {
public:
	XSAnimatedPoint() = delete;
	XSAnimatedPoint(double x, double y, int T);		/*构造函数*/
	~XSAnimatedPoint();							/*析构函数*/
	
	XSPoint *get_pos();							/*获得对象坐标*/
	virtual void on_loop(int n);				/*响应循环(每周期要干的事)*/
	
	void set_func_x(std::function<double(long)> func_x);		/*设置 x 轴方向运动方程*/
	void set_func_y(std::function<double(long)> func_y);		/*设置 y 轴方向运动方程*/
private:
	XSPoint m_pos;								/*点的坐标*/
	std::function<double(long)> m_func_x;			/*x 轴方向运动方程*/
	std::function<double(long)> m_func_y;			/*y 轴方向运动方程*/
};

#endif
