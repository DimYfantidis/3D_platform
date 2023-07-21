#pragma once

#include <iostream>
#include <GL/glut.h>

#include "logging.h"
#include "typedefs.h"


class Cuboid
{
public:
	Cuboid(float width, float height, float depth)
		: m_width(width), m_height(height), m_depth(depth)
	{
		m_color[0] = 1.0f;
		m_color[1] = 1.0f;
		m_color[2] = 1.0f;
	}

	void spawnAt(float x, float y, float z) 
	{
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
	}

	void spawnAt(vector3f p) {
		this->spawnAt(p[0], p[1], p[2]);
	}
	
	Cuboid& color(float red, float green, float blue) {
		m_color[0] = red;
		m_color[1] = green;
		m_color[2] = blue;
		return (*this);
	}

private:
	float m_width;
	float m_height;
	float m_depth;

	vector3f m_color;
};