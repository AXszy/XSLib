/**
 * @file xsmovable.cpp 
 * @brief: 可移动对象的实现
 * @author: XSSunet
 * @date: 2023-07-16 01:00:13
 * 
 **/

#include "xsmovable.h"
#include <windows.h>

XSMovable::XSMovable()
{
	m_pos.x = 0.5;
	m_pos.y = 0.5;
	
	b_in = false;
	b_push = false;
}

XSMovable::XSMovable(double x, double y)
{
	m_pos.x = x;
	m_pos.y = y;
	
	b_in = false;
	b_push = false;
}

XSMovable::~XSMovable()
{
	
}

void XSMovable::move_to(double x, double y)
{
	if(b_push)
	{
		this->m_move_to(x, y);
		this->m_pos.x = x;
		this->m_pos.y = y;
	}
	else this->m_subobjects_move_to(x, y);
}

void XSMovable::draw(XSGraphic *g)
{
	this->m_draw(g, b_in, b_push);
}

bool XSMovable::b_in_range(double x, double y)
{
	b_in = !this->m_b_in_subobject_range(x, y);
	b_in &= this->m_delta(x, y);
	return b_in;
}

void XSMovable::push()
{
	b_push = true;
}

void XSMovable::pop()
{
	b_push = false;
}

XSPoint *XSMovable::get_pos()
{
	return &this->m_pos;
}

void XSMovable::set_pos(double x, double y)
{
	this->m_pos.x = x;
	this->m_pos.y = y;
}

bool XSMovable::m_b_in_subobject_range(double, double)
{
	return false;
}

void XSMovable::m_subobjects_move_to(double, double)
{
	
}

void XSMovable::m_move_to(double x, double y)
{
	
}
