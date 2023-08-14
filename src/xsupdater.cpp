/**
 * @file xsupdater.cpp 
 * @brief: 更新器类的实现
 * @author: XSSunet
 * @date: 2023-08-03 00:22:27
 * 
 **/
#include "xsupdater.h"
#include <windows.h>

/*double 类型的更新器*/

XSUpdaterDouble::XSUpdaterDouble(double *obj, double delta_num, int time)
{
	this->m_obj = obj;
	this->m_delta_num = delta_num;
	this->m_time = time;
	this->m_b_pause = true;
	QueryPerformanceCounter(&this->m_initial_time);
}

XSUpdaterDouble::~XSUpdaterDouble()
{
	
}

void XSUpdaterDouble::set_delta_num(double dnum)
{
	this->m_delta_num = dnum;
}

void XSUpdaterDouble::set_initial_num(double init_num)
{
	this->m_initial_num = init_num;
}

void XSUpdaterDouble::on_timer()
{
	LARGE_INTEGER current_time, last_time = this->m_initial_time, t_freq;
	//获得当前时间
	QueryPerformanceCounter(&current_time);
	//获得频率
	QueryPerformanceFrequency(&t_freq);
	
	if(this->m_b_pause)
	{
		//将起始时间往后推
		this->m_initial_time.QuadPart += (current_time.QuadPart - last_time.QuadPart);
	}
	else
	{
		//计算两次时间的差值
		long dt = (current_time.QuadPart - last_time.QuadPart) / t_freq.QuadPart;
		//更新对象的值(线性插值)
		*this->m_obj = this->m_initial_num + dt * this->m_delta_num / this->m_time;
		//如果更新完毕就暂停
		if(dt > this->m_time)
		{
			*this->m_obj = this->m_initial_num + this->m_delta_num;
			this->m_initial_num = *this->m_obj;
			this->m_b_pause = true;
		}
	}
	
}

void XSUpdaterDouble::start()
{
	this->m_b_pause = false;
	//获得当前时间
	QueryPerformanceCounter(&this->m_initial_time);
}

void XSUpdaterDouble::resume()
{
	this->m_b_pause = false;
}

void XSUpdaterDouble::pause()
{
	this->m_b_pause = true;
}

/*long 类型的更新器*/

XSUpdaterLong::XSUpdaterLong(long *obj, long delta_num, int time)
{
	this->m_obj = obj;
	this->m_delta_num = delta_num;
	this->m_time = time;
	this->m_b_pause = true;
	QueryPerformanceCounter(&this->m_initial_time);
}

XSUpdaterLong::~XSUpdaterLong()
{
	
}

void XSUpdaterLong::set_delta_num(long dnum)
{
	this->m_delta_num = dnum;
}

void XSUpdaterLong::set_initial_num(long init_num)
{
	this->m_initial_num = init_num;
}

void XSUpdaterLong::on_timer()
{
	LARGE_INTEGER current_time, last_time = this->m_initial_time, t_freq;
	//获得当前时间
	QueryPerformanceCounter(&current_time);
	//获得频率
	QueryPerformanceFrequency(&t_freq);
	
	if(this->m_b_pause)
	{
		//将起始时间往后推
		this->m_initial_time.QuadPart += (current_time.QuadPart - last_time.QuadPart);
	}
	else
	{
		//计算两次时间的差值
		long dt = (current_time.QuadPart - last_time.QuadPart) / t_freq.QuadPart;
		//更新对象的值(线性插值)
		*this->m_obj = this->m_initial_num + dt * this->m_delta_num / this->m_time;
		//如果更新完毕就暂停
		if(dt > this->m_time)
		{
			*this->m_obj = this->m_initial_num + this->m_delta_num;
			this->m_initial_num = *this->m_obj;
			this->m_b_pause = true;
		}
	}
	
}

void XSUpdaterLong::start()
{
	this->m_b_pause = false;
	//获得当前时间
	QueryPerformanceCounter(&this->m_initial_time);
}

void XSUpdaterLong::resume()
{
	this->m_b_pause = false;
}

void XSUpdaterLong::pause()
{
	this->m_b_pause = true;
}

