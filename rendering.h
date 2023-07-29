#pragma once


#include <array>
#include <GL/freeglut.h>

#include "typedefs.h"


namespace engine
{
	void renderString(float x, float y, void* font, const char* string, const vector3f rgb)
	{
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		{
			const char* c;

			glColor3f(rgb[0], rgb[1], rgb[2]);
			glRasterPos2f(x, y);

			glutBitmapString(font, (const unsigned char*)string);
		}
		glPopAttrib();
	}
	void renderString(float x, float y, void* font, const char* string, const std::array<float, 3>& rgb) {
		renderString(x, y, font, string, rgb.data());
	}
}
