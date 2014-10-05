#include <algorithm>

#pragma once

class Town;

class Line
{
/*****************************************************************************/
/*                        User Defined Variables                             */
/*****************************************************************************/
public:

private:
	enum Orientation
	{
		colinear,
		clockwise,
		counterclockwise
	};

	struct Point
	{
		int x;
		int y;
	};
/*****************************************************************************/
/*                               Functions                                   */
/*****************************************************************************/
public:
	Line(const Town* A, const Town* B);

	double GetDistance();
	static double GetDistance(const Town* x, const Town* y);

	static bool AreIntersected(const Line line1, const Line line2);
private:
	void SetVariables();

	static bool IsOnSegment(const Point p, const Point q, const Point r);

	static Orientation GetOrientation(const Point p, const Point q, const Point r);

	static bool PointsAreSame(const Point p, const Point q);
	/*****************************************************************************/
	/*                            Class Variables                                */
	/*****************************************************************************/
public:
	const Town* _A;
	const Town* _B;

	Point _p, _q;

	double _a, _b;
};

