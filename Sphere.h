#pragma once

#include "Shape.h"


namespace engine
{
	class Sphere : public shape
	{
	public:
		Sphere(float radius)
			: shape(), m_radius(radius), m_rec_depth(4)
		{}

		Sphere(const Sphere& other)
			: shape(other), m_radius(other.m_radius), m_rec_depth(other.m_rec_depth)
		{}
		Sphere(Sphere&& other)
			: shape(std::move(other)), m_radius(other.m_radius), m_rec_depth(other.m_rec_depth)
		{}

		shape& resolution(int val) override {
			m_rec_depth = val;
			return (*this);
		}

		shape& spawn(float x, float y, float z) override
		{
			if constexpr (LOG_SHAPES)
				logger.logMessage(
					"Spawning sphere at (%.3f, %.3f, %.3f), radius = %.3f",
					x, y, z,
					m_radius,
					m_color[0], m_color[1], m_color[2]
				);

			glPushMatrix();
			{
				glColor3fv(m_color);

				glMaterialf(GL_FRONT, GL_SHININESS, m_shininess);
				glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);

				// The sphere's center is translated to the desired point.
				glTranslatef(x, y, z);

				// Sphere is uniformly scaled to meet its radius.
				glScalef(m_radius, m_radius, m_radius);

				//glEnable(GL_NORMALIZE);
				// Recursive subdivision starts with the 
				// tetrahedron's center at the origin.
				recursiveSubdivision(m_rec_depth);
				//glDisable(GL_NORMALIZE);
			}
			glPopMatrix();

			return (*this);
		}

	private:
		// Recursive sibdivision of a triangle into 4 equilateral triangles.
		void divideTriangle(point3f a, point3f b, point3f c, int m)
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
				for (j = 0; j < 3; j++)
				{
					v1[j] = a[j] + b[j];
					v2[j] = a[j] + c[j];
					v3[j] = b[j] + c[j];
				}
				normalize(v1);
				normalize(v2);
				normalize(v3);

				//Form said 4 new triangles with the new points and continue recursive subdivision
				divideTriangle(a, v1, v2, m - 1);
				divideTriangle(c, v2, v3, m - 1);
				divideTriangle(b, v3, v1, m - 1);
				divideTriangle(v1, v3, v2, m - 1);
			}

			//Draw final points as polygons onto the unit sphere
			else
			{
				crossProduct(cross, c, a, b);
				normalize(cross);

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
		void recursiveSubdivision(int m) {
			//Starting values for the tetrahedron
			//These values are inverted so the sun moves opposite to its light source
			point3f v[] = {
				{-0.0000000f, -0.0000000f, -1.0000000f},
				{-0.0000000f, -0.9428090f, 0.33333300f},
				{0.81649700f, 0.47140500f, 0.33333300f},
				{-0.8164970f, 0.47140500f, 0.33333300f}
			};

			//Take the points of the tetrahedron and divide it into 4 triangles
			//Initiate subdivision on each one of them
			divideTriangle(v[0], v[1], v[2], m);	//First Triangle
			divideTriangle(v[3], v[2], v[1], m);	//Second Triangle
			divideTriangle(v[0], v[3], v[1], m);	//Third Triangle
			divideTriangle(v[0], v[2], v[3], m);	//Fourth Triangle
		}

	private:
		// m_rec_depth represents the depth of the recursive
		// subdivision's tree depth. Default value is 4 but can be increased
		// for larger spheres.
		int m_rec_depth;
		float m_radius;
	};
}