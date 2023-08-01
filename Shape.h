#pragma once

#include <array>
#include <iostream>
#include <GL/glut.h>

#include "logging.h"
#include "typedefs.h"
#include "geometry.h"
#include "materials.h"


extern ScreenLogger logger;

namespace engine
{
	/*	Parent class for all sort of geometric shapes.
	*	Deriving classes include:
	*		- Rectangle (2D)
	*		- Cuboid (3D)
	*		- Sphere (3D)
	*	Methods return shape& for chaining reasons.
	*/
	class shape
	{
	public:
		shape()
			: m_color{ 1.0f, 1.0f, 1.0f }, m_ambient(), m_specular(), m_diffuse(), m_shininess(0.0f)
		{}

		shape(const shape& other) : m_shininess(other.m_shininess)
		{
			memmove(this->m_color, other.m_color, sizeof(vector3f));
			memmove(this->m_ambient, other.m_ambient, sizeof(vector4f));
			memmove(this->m_specular, other.m_specular, sizeof(vector4f));
			memmove(this->m_diffuse, other.m_diffuse, sizeof(vector4f));
		}

		// Spawns the object with its center at the given point.
		// Implementation is done by the derived classes.
		virtual shape& spawn(float x, float y, float z) { return (*this); }
		shape& spawn(const point3f p) { return spawn(p[0], p[1], p[2]); }

		shape& color(float red, float green, float blue)
		{
			m_color[0] = red;
			m_color[1] = green;
			m_color[2] = blue;
			return (*this);
		}

		shape& colorv(const vector3f RGB)
		{
			m_color[0] = RGB[0];
			m_color[1] = RGB[1];
			m_color[2] = RGB[2];
			return (*this);
		}

		shape& shininess(float val) {
			m_shininess = val;
			return (*this);
		}

		// Sets the material properties.
		// Acceptanle values for pname are GL_AMBIENT, GL_SPECULAR and GL_DIFFUSE. */
		shape& materialv(GLenum pname, const vector4f params)
		{
			float* light_type;

			if (pname == GL_AMBIENT)
				light_type = m_ambient;
			else if (pname == GL_SPECULAR)
				light_type = m_specular;
			else if (pname == GL_DIFFUSE)
				light_type = m_diffuse;
			else {
				logger.logWarning("WARNING: undefined lighting type (0x%04x) in shape's \"0x%X\" meterial", pname, this);
				return (*this);
			}
			memmove(light_type, params, sizeof(vector4f));
			return (*this);
		}

		shape& materialv(GLenum pname, const std::array<float, 4>& params) {
			return materialv(pname, params.data());
		}

		shape& material(const materials::material& blueprint)
		{
			m_shininess = blueprint.shininess;
			memmove(m_ambient, blueprint.ambient, sizeof(vector4f));
			memmove(m_specular, blueprint.specular, sizeof(vector4f));
			memmove(m_diffuse, blueprint.diffuse, sizeof(vector4f));
			return (*this);
		}

		virtual shape& resolution(int val) { return (*this); }

		// Getters
		const float* color() { return m_color; }
		const float* ambient() { return m_ambient; }
		const float* specular() { return m_specular; }
		const float* diffuse() { return m_diffuse; }

	protected:
		// Color of geometric shape without lighting.
		vector3f m_color;

		// Material properties for enabled lighting.
		vector4f m_ambient;
		vector4f m_specular;
		vector4f m_diffuse;

		float m_shininess;
	};
}