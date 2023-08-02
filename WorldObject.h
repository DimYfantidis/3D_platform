#pragma once

#include <GL/glut.h>


namespace engine
{
	class WorldObject
	{
	public:
		WorldObject() : CBlist() {}

	public:
		struct CollisionBox 
		{
			float x1;
			float y1;
			float z1;

			float x2;
			float y2;
			float z2;

			void draw() 
			{
				static float color[4] = {1.0f, 0.0f, 0.0f, 0.0f};

				glPushMatrix();

				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);

				glBegin(GL_LINE_LOOP);
				{
					glVertex3f(x1, y1, z1);
					glVertex3f(x2, y1, z1);
					glVertex3f(x2, y1, z2);
					glVertex3f(x1, y1, z2);
					glVertex3f(x1, y1, z1);

					glVertex3f(x1, y2, z1);
					glVertex3f(x2, y2, z1);
					glVertex3f(x2, y2, z2);
					glVertex3f(x1, y2, z2);
					glVertex3f(x1, y2, z1);
				}
				glEnd();

				glBegin(GL_LINES);
				{
					glVertex3f(x1, y1, z2);
					glVertex3f(x1, y2, z2);

					glVertex3f(x2, y1, z1);
					glVertex3f(x2, y2, z1);

					glVertex3f(x2, y1, z2);
					glVertex3f(x2, y2, z2);
				}
				glEnd();

				glPopMatrix();
			}
		};

	public:
		virtual WorldObject& createCollisionBox(float x, float y, float z) { return (*this); }

		virtual WorldObject& showCollisionBox() 
		{
			for (auto bb : CBlist) {
				bb->draw();
			}
			return (*this);
		}

		const std::vector<CollisionBox*>& getCollisionList() const { return CBlist; }

	protected:
		std::vector<CollisionBox*> CBlist;
	};
}