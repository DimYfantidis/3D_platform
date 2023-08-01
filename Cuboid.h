#pragma once

#include "Shape.h"


namespace engine
{
	// The 3D version of a rectangle.
	class Cuboid : public shape
	{
	public:
		Cuboid(float width, float height, float depth)
			: shape(), m_width(width), m_height(height), m_depth(depth),
			m_length_poly_count(1), m_dx(width), m_dy(height), m_dz(depth)
		{}

		Cuboid(const Cuboid& other)
			: shape(other), m_width(other.m_width), m_height(other.m_height), m_depth(other.m_depth),
			m_length_poly_count(1), m_dx(other.m_width), m_dy(other.m_height), m_dz(other.m_depth)
		{}


		shape& resolution(int val) override
		{
			m_length_poly_count = val;
			m_dx = m_width / m_length_poly_count;
			m_dy = m_height / m_length_poly_count;
			m_dz = m_depth / m_length_poly_count;
			return (*this);
		}

		shape& spawn(float x, float y, float z) override
		{
			x -= m_width / 2;
			y -= m_height / 2;
			z -= m_depth / 2;

			float x_begin = x;
			float y_begin = y;
			float z_begin = z;

			glPushMatrix();

			if constexpr (LOG_SHAPES)
				logger.logMessage(
					"Spawning cuboid at (%.3f, %.3f, %.3f), dim=(%.3f, %.3f, %.3f)",
					x, y, z,
					m_width, m_height, m_depth
				);

			glColor3fv(m_color);

			glMaterialf(GL_FRONT, GL_SHININESS, m_shininess);
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);

			int i, j;

			// Consists of 6 rectangles(12 triangles).
			glBegin(GL_TRIANGLE_STRIP);
			{
				glNormal3f(0.0f, 0.0f, -1.0f);
				for (i = 0; i < m_length_poly_count; ++i)
				{
					y = y_begin;
					for (j = 0; j < m_length_poly_count; ++j)
					{
						glVertex3f(x, y, z);
						glVertex3f(x, y + m_dy, z);
						glVertex3f(x + m_dx, y, z);
						glVertex3f(x + m_dx, y + m_dy, z);
						y = y + m_dy;
					}
					x = x + m_dx;
				}
				x = x_begin;
				y = y_begin;
			}
			glEnd();
			glBegin(GL_TRIANGLE_STRIP);
			{
				glNormal3f(-1.0f, 0.0f, 0.0f);
				for (i = 0; i < m_length_poly_count; ++i)
				{
					y = y_begin;
					for (j = 0; j < m_length_poly_count; ++j)
					{
						glVertex3f(x, y, z);
						glVertex3f(x, y, z + m_dz);
						glVertex3f(x, y + m_dy, z);
						glVertex3f(x, y + m_dy, z + m_dz);
						y = y + m_dy;
					}
					z = z + m_dz;
				}
				x = x_begin;
				z = z_begin;
				y = y_begin + m_height;
			}
			glEnd();
			glBegin(GL_TRIANGLE_STRIP);
			{
				glNormal3f(0.0f, 1.0f, 0.0f);
				for (i = 0; i < m_length_poly_count; ++i)
				{
					z = z_begin;
					for (j = 0; j < m_length_poly_count; ++j)
					{
						glVertex3f(x, y, z);
						glVertex3f(x + m_dx, y, z);
						glVertex3f(x, y, z + m_dz);
						glVertex3f(x + m_dx, y, z + m_dz);
						z = z + m_dz;
					}
					x = x + m_dx;
				}
				x = x_begin + m_width;
				z = z_begin;
				y = y_begin;
			}
			glEnd();
			glBegin(GL_TRIANGLE_STRIP);
			{
				glNormal3f(1.0f, 0.0f, 0.0f);
				for (i = 0; i < m_length_poly_count; ++i)
				{
					z = z_begin;
					for (j = 0; j < m_length_poly_count; ++j)
					{
						glVertex3f(x, y, z);
						glVertex3f(x, y, z + m_dz);
						glVertex3f(x, y + m_dy, z);
						glVertex3f(x, y + m_dy, z + m_dz);
						z = z + m_dz;
					}
					y = y + m_dy;
				}
			}
			glEnd();
			glBegin(GL_TRIANGLE_STRIP);
			{
				x = x_begin;
				z = z_begin;
				y = y_begin;

				glNormal3f(0.0f, -1.0f, 0.0f);
				for (i = 0; i < m_length_poly_count; ++i)
				{
					z = z_begin;
					for (j = 0; j < m_length_poly_count; ++j)
					{
						glVertex3f(x, y, z);
						glVertex3f(x + m_dx, y, z);
						glVertex3f(x, y, z + m_dz);
						glVertex3f(x + m_dx, y, z + m_dz);
						z = z + m_dz;
					}
					x = x + m_dx;
				}
				x = x_begin;
				z = z_begin + m_depth;
				y = y_begin;
			}
			glEnd();
			glBegin(GL_TRIANGLE_STRIP);
			{
				glNormal3f(0.0f, 0.0f, 1.0f);
				for (i = 0; i < m_length_poly_count; ++i)
				{
					y = y_begin;
					for (j = 0; j < m_length_poly_count; ++j)
					{
						glVertex3f(x, y, z);
						glVertex3f(x + m_dx, y, z);
						glVertex3f(x, y + m_dy, z);
						glVertex3f(x + m_dx, y + m_dy, z);
						y = y + m_dy;
					}
					x = x + m_dx;
				}
			}
			glEnd();

			glPopMatrix();

			return (*this);
		}

	private:
		int m_length_poly_count;
		float m_dx;
		float m_dy;
		float m_dz;
		float m_width;
		float m_height;
		float m_depth;
	};
}