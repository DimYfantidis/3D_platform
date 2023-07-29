#pragma once

#include <array>
#include <iostream>
#include <GL/glut.h>

#include "logging.h"
#include "typedefs.h"
#include "geometry.h"


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
			: m_color(), m_ambient(), m_specular(), m_diffuse()
		{}
		
		shape(const shape& other)
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
	};


	// Rectangles have width (x) and depth (z) but no height (y_top = y_bottom)
	class rectangle : public shape
	{
	public:
		rectangle(float width, float depth)
			: shape(), m_width(width), m_depth(depth)
		{}

		rectangle(const rectangle& other)
			: shape(other), m_width(other.m_width), m_depth(other.m_depth)
		{}

		shape& spawn(float x, float y, float z) override
		{
			x -= m_width / 2;
			z -= m_depth / 2;

			glPushMatrix();

			if constexpr (LOG_SHAPES)
				logger.logMessage(
					"Spawning rectangle at (%.3f, %.3f, %.3f), dim=(%.3f, %.3f, %.3f)",
					x, y, z,
					m_width, 0.0f, m_depth,
					m_color[0], m_color[1], m_color[2]
				);

			glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);


			// Rectangle consists of two congruent triangles (better speed)
			glBegin(GL_TRIANGLES);
			{
				glNormal3f(0.0f, 1.0f, 0.0f);

				// 1
				glVertex3f(x, y, z);
				glVertex3f(x + m_width, y, z + m_depth);
				glVertex3f(x, y, z + m_depth);
				
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


	// The 3D version of a rectangle.
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

			if constexpr (LOG_SHAPES)
				logger.logMessage(
					"Spawning cuboid at (%.3f, %.3f, %.3f), dim=(%.3f, %.3f, %.3f)", 
					x, y, z,
					m_width, m_height, m_depth,
					m_color[0], m_color[1], m_color[2]
				);

			glColor3fv(m_color);
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);

			// Consists of 6 rectangles(12 triangles).
			glBegin(GL_TRIANGLES);
			{
				glNormal3f(0.0f, 0.0f, 1.0f);
				// 1
				glVertex3f(x, y, z);
				glVertex3f(x + m_width, y, z);
				glVertex3f(x + m_width, y + m_height, z);

				// 2
				glVertex3f(x, y, z);
				glVertex3f(x, y + m_height, z);
				glVertex3f(x + m_width, y + m_height, z);

				glNormal3f(-1.0f, 0.0f, 0.0f);
				// 3
				glVertex3f(x, y, z);
				glVertex3f(x, y + m_height, z);
				glVertex3f(x, y + m_height, z + m_depth);

				// 4
				glVertex3f(x, y, z);
				glVertex3f(x, y, z + m_depth);
				glVertex3f(x, y + m_height, z + m_depth);

				glNormal3f(0.0f, 0.0f, -1.0f);
				// 5
				glVertex3f(x, y + m_height, z);
				glVertex3f(x, y + m_height, z + m_depth);
				glVertex3f(x + m_width, y + m_height, z + m_depth);

				// 6
				glVertex3f(x, y + m_height, z);
				glVertex3f(x + m_width, y + m_height, z);
				glVertex3f(x + m_width, y + m_height, z + m_depth);

				glNormal3f(1.0f, 0.0f, 0.0f);
				// 7
				glVertex3f(x + m_width, y + m_height, z);
				glVertex3f(x + m_width, y, z);
				glVertex3f(x + m_width, y, z + m_depth);

				// 8
				glVertex3f(x + m_width, y + m_height, z);
				glVertex3f(x + m_width, y + m_height, z + m_depth);
				glVertex3f(x + m_width, y, z + m_depth);

				glNormal3f(0.0f, -1.0f, 0.0f);
				// 9
				glVertex3f(x, y, z);
				glVertex3f(x, y, z + m_depth);
				glVertex3f(x + m_width, y, z + m_depth);

				// 10
				glVertex3f(x, y, z);
				glVertex3f(x + m_width, y, z);
				glVertex3f(x + m_width, y, z + m_depth);

				glNormal3f(0.0f, 1.0f, 0.0f);
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


	// Sphere class. REC_DEPTH represents the depth of the recursive 
	// subdivision's tree depth. Default value is 4 but can be increased
	// for larger spheres.
	template <int REC_DEPTH = 4>
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

				glMaterialfv(GL_FRONT, GL_AMBIENT, m_ambient);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_specular);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diffuse);

				// The sphere's center is translated to the desired point.
				glTranslatef(x, y, z);

				// Sphere is uniformly scaled to meet its radius.
				glScalef(m_radius, m_radius, m_radius);

				// Recursive subdivision starts with the 
				// tetrahedron's center at the origin.
				tetrahedron(REC_DEPTH);
			}
			glPopMatrix();

			return (*this);
		}

	private:
		// Recursive sibdivision of a triangle into 4 equilateral triangles.
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
				for (j = 0; j < 3; j++)
				{
					v1[j] = a[j] + b[j];
				}
				for (j = 0; j < 3; j++)
				{
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
				crossProduct(cross,
					c[0], c[1], c[2],
					a[0], a[1], a[2],
					b[0], b[1], b[2]
				);

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
				{-0.0000000f, -0.0000000f, -1.0000000f},
				{-0.0000000f, -0.9428090f, 0.333333f},
				{0.816497f, 0.471405f, 0.333333f},
				{-0.8164970f, 0.471405f, 0.333333f}
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


	class light_source
	{
	public:
		light_source(GLenum id, float radius = 1.0f) 
			: m_light_id(id), m_emission(), 
			m_ambient(), m_specular(), m_diffuse(), 
			m_cell(new sphere(radius))
		{}

		light_source(GLenum id, float width, float height, float depth)
			: m_light_id(id), m_emission(), 
			m_ambient(), m_specular(), m_diffuse(), 
			m_cell(new cuboid(width, height, depth))
		{}

		~light_source() {
			delete m_cell;
		}

		light_source& emission(const vector4f params) {
			memmove(m_emission, params, sizeof(vector4f));
			return (*this);
		}

		light_source& emission(std::array<float, 4> params) {
			return emission(params.data());
		}

		light_source& materialv(GLenum pname, const std::array<float, 4>& params) {
			m_cell->materialv(pname, params);
			return (*this);
		}

		light_source& lightv(GLenum pname, const vector4f params) 
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

		light_source& lightv(GLenum pname, const std::array<float, 4>& params) {
			return lightv(pname, params.data());
		}

		light_source& spawn(float x, float y, float z) 
		{
			vector4f position = { x, y, z, 0.0f};

			glPushMatrix();
			{
				//Create sun's materials for color and light
				glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, m_emission);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0);

				m_cell->spawn(x, y, z);

				//Create light as a directional spotlight
				glLightfv(m_light_id, GL_POSITION, position);
				glLightfv(m_light_id, GL_DIFFUSE, m_diffuse);
				glLightfv(m_light_id, GL_SPECULAR, m_specular);
			}
			glPopMatrix();
			
			return (*this);
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
