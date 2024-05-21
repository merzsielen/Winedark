#ifndef POLYGON_H
#define POLYGON_H

namespace Winedark
{
	struct Vertex
	{
		/*-----------------------*/
		/* Coordinates           */
		/*-----------------------*/
		float	x;
		float	y;
		float	z;

		/*-----------------------*/
		/* Color                 */
		/*-----------------------*/
		float	r;
		float	g;
		float	b;
		float	a;
	};

	struct Triangle
	{
		Vertex	a;
		Vertex	b;
		Vertex	c;
	};
}

#endif