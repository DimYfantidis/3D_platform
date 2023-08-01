#pragma once

#include "Shape.h"
#include "Sphere.h"
#include "Cuboid.h"


namespace engine 
{
	class LightSource
	{
	public:
		LightSource(GLenum id, float radius = 1.0f)
			: m_light_id(id), m_emission(),
			m_ambient(), m_specular(), m_diffuse(),
			m_cell(new Sphere(radius))
		{}

		LightSource(GLenum id, float width, float height, float depth)
			: m_light_id(id), m_emission(),
			m_ambient(), m_specular(), m_diffuse(),
			m_cell(new Cuboid(width, height, depth))
		{}

		~LightSource() {
			delete m_cell;
		}

		LightSource& emission(const vector4f params) {
			memmove(m_emission, params, sizeof(vector4f));
			return (*this);
		}

		LightSource& emission(std::array<float, 4> params) {
			return emission(params.data());
		}

		LightSource& materialv(GLenum pname, const vector4f params) {
			m_cell->materialv(pname, params);
			return (*this);
		}
		LightSource& materialv(GLenum pname, const std::array<float, 4>& params) {
			return materialv(pname, params.data());
		}

		LightSource& lightv(GLenum pname, const vector4f params)
		{
			float* light_type;

			if (pname == GL_AMBIENT)
				light_type = m_ambient;
			else if (pname == GL_SPECULAR)
				light_type = m_specular;
			else if (pname == GL_DIFFUSE)
				light_type = m_diffuse;
			else {
				logger.logWarning(
					"WARNING: undefined lighting type in shape's \"%X\" meterial", this);
				return (*this);
			}
			memmove(light_type, params, sizeof(vector4f));

			return (*this);
		}

		LightSource& lightv(GLenum pname, const std::array<float, 4>& params) {
			return lightv(pname, params.data());
		}

		LightSource& spawn(float x, float y, float z)
		{
			vector4f position = { x, y, z, 1.0f };

			glPushMatrix();
			{
				//Create sun's materials for color and light
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission);

				m_cell->shininess(100).spawn(x, y, z);

				//Create light as a directional spotlight
				glLightfv(m_light_id, GL_POSITION, position);
				glLightfv(m_light_id, GL_DIFFUSE, m_diffuse);
				glLightfv(m_light_id, GL_SPECULAR, m_specular);
			}
			glPopMatrix();

			return (*this);
		}
		LightSource& spawn(const vector3f pos) {
			return spawn(pos[0], pos[1], pos[2]);
		}

	private:
		vector4f m_emission;

		vector4f m_ambient;
		vector4f m_specular;
		vector4f m_diffuse;

		GLenum m_light_id;
		shape* m_cell;
	};
}