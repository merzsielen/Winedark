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

		/*-----------------------*/
		/* Texture               */
		/*-----------------------*/
		float	u;
		float	v;
		float	t;
	};

	struct Triangle
	{
		Vertex	a;
		Vertex	b;
		Vertex	c;
	};
}

#endif