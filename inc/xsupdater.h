/**
 * @file xsupdater.h 
 * @brief: 更新器
 * @author: XSSunet
 * @date: 2023-08-03 00:22:13
 * 
 **/
#ifndef XSUPDATER_H
#define XSUPDATER_H

#include <windows.h>

/**
 * @file xsupdater.h 
 * @brief: double 类型的更新器
 * @author: XSSunet
 * @date: 2023-08-08 08:19:49
 * 
 **/
/*double 类型的更新器*/
class XSUpdaterDouble {
public:
	XSUpdaterDouble() = delete;
	XSUpdaterDouble(double *obj, double delta_num, int time);		/*构造函数*/
	~XSUpdaterDouble();
	
	void set_initial_num(double init_num);					/*设置初始值*/
	void set_delta_num(double dnum);						/*设置变化量*/
	void on_timer();										/*响应计时器*/
	void start();											/*启动计时器*/
	void resume();											/*继续更新器*/
	void pause();											/*暂停更新器*/
private:
	double *m_obj;											/*要更新的对象指针*/
	double m_initial_num;									/*初始值*/
	double m_delta_num;										/*与目标的差距*/
	int m_time;												/*动画时间*/
	bool m_b_pause;											/*是否暂停*/
	LARGE_INTEGER m_initial_time;							/*动画启动时间*/
};


/**
 * @file xsupdater.h 
 * @brief: long 类型的更新器
 * @author: XSSunet
 * @date: 2023-08-08 08:20:24
 * 
 **/
/*long 类型的更新器*/
class XSUpdaterLong {
public:
	XSUpdaterLong() = delete;
	XSUpdaterLong(long *obj, long delta_num, int time);		/*构造函数*/
	~XSUpdaterLong();
	
	void set_initial_num(long init_num);					/*设置初始值*/
	void set_delta_num(long dnum);							/*设置变化量*/
	void on_timer();										/*响应计时器*/
	void start();											/*启动计时器*/
	void resume();											/*继续更新器*/
	void pause();											/*暂停更新器*/
private:
	long *m_obj;											/*要更新的对象指针*/
	long m_initial_num;										/*初始值*/
	long m_delta_num;										/*与目标的差距*/
	int m_time;												/*动画时间*/
	bool m_b_pause;											/*是否暂停*/
	LARGE_INTEGER m_initial_time;							/*动画启动时间*/
};

#endif
