#include "Town.h"
#include "Line.h"

Line::Line(const Town* A, const Town* B)
:
_A(A),
_B(B)
{
	SetVariables();
}

void Line::SetVariables()
{
	_p.x = _A->_row;
	_p.y = _A->_column;

	_q.x = _B->_row;
	_q.y = _B->_column;

	_a = (double) (_q.y - _p.y) / (_q.x - _p.x);

	_b = (double) (_a * _p.x) + _p.y;
}

double Line::GetDistance(const Town* x, const Town* y)
{
	const double x1 = x->_row;
	const double y1 = x->_column;

	const double x2 = y->_row;
	const double y2 = y->_column;

	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double Line::GetDistance()
{
	const double x1 = _p.x;
	const double y1 = _p.y;

	const double x2 = _q.x;
	const double y2 = _q.y;

	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

bool Line::AreIntersected(const Line line1, const Line line2)
{
	const Point p1 = line1._p;
	const Point q1 = line1._q;

	const Point p2 = line2._p;
	const Point q2 = line2._q;

	Line::Orientation o1 = GetOrientation(p1, q1, p2);
	Line::Orientation o2 = GetOrientation(p1, q1, q2);
	Line::Orientation o3 = GetOrientation(p2, q2, p1);
	Line::Orientation o4 = GetOrientation(p2, q2, q1);



	if (PointsAreSame(p1, p2) ||
		PointsAreSame(p1, q2) ||
		PointsAreSame(q1, p2) ||
		PointsAreSame(q1, q2))
	{
		return false;
	}
	else
	{
		if (o1 != o2 && o3 != o4)
		{
			return true;
		}

		// Special Cases

		// p1, q1 and p2 are colinear and p2 lies on segment p1q1
		if (o1 == Orientation::colinear && IsOnSegment(p1, p2, q1))
		{
			return true;
		}
		// p1, q1 and p2 are colinear and q2 lies on segment p1q1
		else if (o2 == Orientation::colinear && IsOnSegment(p1, q2, q1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
/*****************************************************************************/
/*                           Point Functions                                 */
/*****************************************************************************/
bool Line::IsOnSegment(const Point p, const Point q, const Point r)
{
	if (q.x <= max(p.x, r.x) && 
		q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && 
		q.y >= min(p.y, r.y))
	{
		return true;
	}
	else
	{
		return false;
	}
}

Line::Orientation Line::GetOrientation(const Point p, const Point q, const Point r)
{
	const int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (val == 0)
	{
		return Orientation::colinear;
	}
	else if (val > 0)
	{
		return Orientation::clockwise;
	}
	else
	{
		return Orientation::counterclockwise;
	}
}

bool Line::PointsAreSame(const Point p, const Point q)
{
	const int x1 = p.x;
	const int y1 = p.y;

	const int x2 = q.x;
	const int y2 = q.y;

	if (x1 == x2 && y1 == y2)
	{
		return true;
	}
	else
	{
		return false;
	}
}