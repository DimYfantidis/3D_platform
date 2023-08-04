#pragma once

#include "Shape.h"


namespace engine 
{
	// Rectangles have width (x) and depth (z) but no height (y_top = y_bottom)
	class Rectangle : public Shape
	{
	public:
		Rectangle(float width, float depth)
			: Shape(), m_width(width), m_depth(depth),
			m_length_poly_count(1), m_dx(width), m_dz(depth)
		{}

		Rectangle(const Rectangle& other)
			: Shape(other), m_width(other.m_width), m_depth(other.m_depth),
			m_length_poly_count(other.m_length_poly_count), m_dx(other.m_width), m_dz(other.m_depth)
		{}

		Rectangle(Rectangle&& other) noexcept
			: Shape(std::move(other)), m_width(other.m_width), m_depth(other.m_depth),
			m_length_poly_count(other.m_length_poly_count), m_dx(other.m_width), m_dz(other.m_depth)
		{}

		Shape& resolution(int val) override
		{
			m_length_poly_count = val;
			m_dx = m_width / m_length_poly_count;
			m_dz = m_depth / m_length_poly_count;
			return (*this);
		}

		Shape& spawn(float x, float y, float z) override
		{
			x -= m_width / 2;
			z -= m_depth / 2;

			float x_begin = x;
			float z_begin = z;

			if constexpr (LOG_SHAPES)
				logger.logMessage(
					"Spawning rectangle at (%.3f, %.3f, %.3f), dim=(%.3f, %.3f, %.3f)",
					x, y, z,
					m_width, 0.0f, m_depth,
					m_color[0], m_color[1], m_color[2]
				);

			glPushMatrix();

			glMaterialf(GL_FRONT, GL_SHININESS, m_shininess);
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);


			// Rectangle consists of two congruent triangles (better speed)
			glBegin(GL_TRIANGLE_STRIP);
			{
				glNormal3f(0.0f, 1.0f, 0.0f);

				int i, j;

				for (i = 0; i < m_length_poly_count; ++i)
				{
					z = z_begin;
					for (j = 0; j < m_length_poly_count; ++j)
					{
						glVertex3f(x, y, z);
						glVertex3f(x, y, z + m_dz);
						glVertex3f(x + m_dx, y, z);
						glVertex3f(x + m_dx, y, z + m_dz);
						z = z + m_dz;
					}
					x = x + m_dx;
				}
			}
			glEnd();

			glPopMatrix();

			return (*this);
		}

		WorldObject& createCollisionBox(float x, float y, float z) override
		{
			auto* AABB = new CollisionBox;

			AABB->x1 = x - m_width / 2;
			AABB->x2 = AABB->x1 + m_width;

			AABB->y1 = y;
			AABB->y2 = y;

			AABB->z1 = z - m_depth / 2;
			AABB->z2 = AABB->z1 + m_depth;

			CBlist.push_back(AABB);

			return (*this);
		}

	private:
		int m_length_poly_count;
		float m_dx;
		float m_dz;
		float m_width;
		float m_depth; 
	};
}
