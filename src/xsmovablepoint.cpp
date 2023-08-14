/**
 * @file xsmovablepoint.cpp 
 * @brief: 可移动的点实现
 * @author: XSSunet
 * @date: 2023-07-16 11:28:10
 * 
 **/
#include "xsmovablepoint.h"
#include <windows.h>
#include "xsgraphic.h"

XSMovablePoint::XSMovablePoint()
{
	m_r = 0.01;
}

XSMovablePoint::XSMovablePoint(double x, double y, double r) : XSMovable(x, y)
{
	m_r = r;
}

XSMovablePoint::~XSMovablePoint()
{
	
}

bool XSMovablePoint::m_delta(double x, double y)
{
	XSPoint *p = this->get_pos();
	return (p->x >= x - m_r) && (p->x <= x + m_r) && (p->y >= y - m_r) && (p->y <= y + m_r);
}

void XSMovablePoint::m_draw(XSGraphic *g, bool bin, bool bpush)
{
	XSPoint *p = this->get_pos();
	
	if(bpush || bin)
	{
		g->rectangle(p->x - m_r, p->y - m_r, p->x + m_r, p->y + m_r);
	}
	else
	{
		g->gradient_rect({p->x - m_r, p->y - m_r}, RGB(17, 177, 0), 
			{p->x + m_r, p->y + m_r}, RGB(51, 201, 230), GRADIENT_FILL_RECT_V);
	}
}
