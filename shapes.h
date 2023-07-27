#pragma once

#include <iostream>
#include <GL/glut.h>

#include "logging.h"
#include "typedefs.h"
#include "geometry.h"


namespace engine
{
	class shape
	{
	public:
		shape()
		{
			m_color[0] = 1.0f;
			m_color[1] = 1.0f;
			m_color[2] = 1.0f;
		}

		shape(const shape& other)
		{
			this->m_color[0] = other.m_color[0];
			this->m_color[1] = other.m_color[1];
			this->m_color[2] = other.m_color[2];
		}

		// Spawns the object with its center at the given point
		virtual shape& spawn(float x, float y, float z) { return (*this); }
		shape& spawn(point3f p) { return spawn(p[0], p[1], p[2]); }

		shape& color(float red, float green, float blue) 
		{
			m_color[0] = red;
			m_color[1] = green;
			m_color[2] = blue;
			return (*this);
		}

	protected:
		vector3f m_color;
	};


	class rectangle : public shape
	{
	public:
		rectangle(float width, float depth)
			: shape(), m_width(width), m_depth(depth)
		{}

		rectangle(const rectangle& other)
			: shape(other), m_width(other.m_width), m_depth(other.m_depth)
		{}

		shape& spawn(float x, float y, float z)
		{
			x -= m_width / 2;
			z -= m_depth / 2;

			glPushMatrix();

			logMessage("Spawning rectangle\n"
				"> pos={%.3f, %.3f, %.3f}\n"
				"> dim={%.3f, %.3f, %.3f}\n"
				"> color={%.3f, %.3f, %.3f}\n",
				x, y, z,
				m_width, 0.0f, m_depth,
				m_color[0], m_color[1], m_color[2]
			);

			glColor3fv(m_color);

			glBegin(GL_TRIANGLES);
			{
				// 1
				glVertex3f(x, y, z);
				glVertex3f(x, y, z + m_depth);
				glVertex3f(x + m_width, y, z + m_depth);

				// 2
				glVertex3f(x, y, z);
				glVertex3f(x + m_width, y, z);
				glVertex3f(x + m_width, y, z + m_depth);
			}
			glEnd();

			glPopMatrix();

			return (*this);
		}

	private:
		float m_width;
		float m_depth;
	};


	class cuboid : public shape
	{
	public:
		cuboid(float width, float height, float depth)
			: shape(), m_width(width), m_height(height), m_depth(depth)
		{}

		cuboid(const cuboid& other)
			: shape(other), m_width(other.m_width), m_height(other.m_height), m_depth(other.m_depth)
		{}

		shape& spawn(float x, float y, float z) override
		{
			x -= m_width / 2;
			y -= m_height / 2;
			z -= m_depth / 2;

			glPushMatrix();

			logMessage("Spawning cuboid\n"
				"> pos={%.3f, %.3f, %.3f}\n"
				"> dim={%.3f, %.3f, %.3f}\n"
				"> color={%.3f, %.3f, %.3f}\n",
				x, y, z,
				m_width, m_height, m_depth,
				m_color[0], m_color[1], m_color[2]
			);

			glColor3fv(m_color);

			glBegin(GL_TRIANGLES);
			{
				// 1
				glVertex3f(x, y, z);
				glVertex3f(x + m_width, y, z);
				glVertex3f(x + m_width, y + m_height, z);

				// 2
				glVertex3f(x, y, z);
				glVertex3f(x, y + m_height, z);
				glVertex3f(x + m_width, y + m_height, z);

				// 3
				glVertex3f(x, y, z);
				glVertex3f(x, y + m_height, z);
				glVertex3f(x, y + m_height, z + m_depth);

				// 4
				glVertex3f(x, y, z);
				glVertex3f(x, y, z + m_depth);
				glVertex3f(x, y + m_height, z + m_depth);

				// 5
				glVertex3f(x, y + m_height, z);
				glVertex3f(x, y + m_height, z + m_depth);
				glVertex3f(x + m_width, y + m_height, z + m_depth);

				// 6
				glVertex3f(x, y + m_height, z);
				glVertex3f(x + m_width, y + m_height, z);
				glVertex3f(x + m_width, y + m_height, z + m_depth);

				// 7
				glVertex3f(x + m_width, y + m_height, z);
				glVertex3f(x + m_width, y, z);
				glVertex3f(x + m_width, y, z + m_depth);

				// 8
				glVertex3f(x + m_width, y + m_height, z);
				glVertex3f(x + m_width, y + m_height, z + m_depth);
				glVertex3f(x + m_width, y, z + m_depth);

				// 9
				glVertex3f(x, y, z);
				glVertex3f(x, y, z + m_depth);
				glVertex3f(x + m_width, y, z + m_depth);

				// 10
				glVertex3f(x, y, z);
				glVertex3f(x + m_width, y, z);
				glVertex3f(x + m_width, y, z + m_depth);

				// 11
				glVertex3f(x, y, z + m_depth);
				glVertex3f(x, y + m_height, z + m_depth);
				glVertex3f(x + m_width, y + m_height, z + m_depth);

				// 12
				glVertex3f(x, y, z + m_depth);
				glVertex3f(x + m_width, y, z + m_depth);
				glVertex3f(x + m_width, y + m_height, z + m_depth);
			}
			glEnd();

			glPopMatrix();

			return (*this);
		}

	private:
		float m_width;
		float m_height;
		float m_depth;
	};

	class sphere : public shape
	{
	public:
		sphere(float radius)
			: shape(), m_radius(radius)
		{}

		sphere(const sphere& other)
			: shape(other), m_radius(other.m_radius)
		{}

		shape& spawn(float x, float y, float z) override 
		{
			glPushMatrix();

			glColor3fv(m_color);
			glTranslatef(x, y, z);
			glScalef(m_radius, m_radius, m_radius);

			tetrahedron(4);

			glPopMatrix();

			return (*this);
		}

	private:
		//Recursive sibdivision of a triangle into 4 equilateral triangles
		void divide_triangle(point3f a, point3f b, point3f c, int m)
		{
			vector3f cross;
			point3f v1, v2, v3;

			int j;

			//Subdivide current triangle
			if (m > 0)
			{
				// Subdivide the triangle into 2 equal parts
				// v1,v2,v3 are the new points created by this subdivision which will form 4 new triangles
				// Normalize these new points so that they move onto the unit sphere
				for (j = 0; j < 3; j++) {
					v1[j] = a[j] + b[j];
				}
				for (j = 0; j < 3; j++) {
					v2[j] = a[j] + c[j];
				}
				for (j = 0; j < 3; j++) {
					v3[j] = b[j] + c[j];
				}
				normalize(v1);
				normalize(v2);
				normalize(v3);

				//Form said 4 new triangles with the new points and continue recursive subdivision
				divide_triangle(a, v1, v2, m - 1);
				divide_triangle(c, v2, v3, m - 1);
				divide_triangle(b, v3, v1, m - 1);
				divide_triangle(v1, v3, v2, m - 1);
			}
			//Draw final points as polygons onto the unit sphere
			else
			{
				cross_product(cross, c, a, b);

				glBegin(GL_TRIANGLES);
				{
					glNormal3fv(cross);
					glVertex3fv(a);
					glVertex3fv(b);
					glVertex3fv(c);
				}
				glEnd();
			}
		}

		//Tetrahedron creation and subdivision initiation
		void tetrahedron(int m) {
			//Starting values for the tetrahedron
			//These values are inverted so the sun moves opposite to its light source
			point3f v[] = {
				{0.0000000f, 0.0000000f, 1.0000000f},
				{0.0000000f, 0.9428090f, -0.333333f},
				{-0.816497f, -0.471405f, -0.333333f},
				{0.8164970f, -0.471405f, -0.333333f}
			};

			//Take the points of the tetrahedron and divide it into 4 triangles
			//Initiate subdivision on each one of them
			divide_triangle(v[0], v[1], v[2], m);	//First Triangle
			divide_triangle(v[3], v[2], v[1], m);	//Second Triangle
			divide_triangle(v[0], v[3], v[1], m);	//Third Triangle
			divide_triangle(v[0], v[2], v[3], m);	//Fourth Triangle
		}

	private:
		float m_radius;
	};
}