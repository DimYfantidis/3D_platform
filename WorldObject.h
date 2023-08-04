#pragma once

#include <string>
#include <cstddef>
#include <GL/glut.h>


namespace engine
{
	class WorldObject
	{
	public:
		inline static volatile size_t instanceCount = 0;

		WorldObject() : CBlist() { instanceCount++; }

		WorldObject(const WorldObject& other): CBlist(other.CBlist.size(), nullptr) 
		{
			instanceCount++;
			for (size_t i = 0; i < CBlist.size(); ++i) {
				CBlist[i] = new CollisionBox(*other.CBlist[i]);
			}
		}

		WorldObject(WorldObject&& other) noexcept : CBlist(std::move(other.CBlist)) {}

		~WorldObject()
		{
			instanceCount--;
			for (auto AABB : CBlist) {
				delete AABB;
			}
		}

	public:
		struct CollisionBox
		{
			float x1;
			float y1;
			float z1;

			float x2;
			float y2;
			float z2;

			CollisionBox() = default;

			CollisionBox(const CollisionBox& other) : 
				x1(other.x1), y1(other.y1), z1(other.z1), 
				x2(other.x2), y2(other.y2), z2(other.z2)
			{}

			~CollisionBox() = default;

			void draw()
			{
				static float color[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
				
				glPushMatrix();
				glPushAttrib(GL_ALL_ATTRIB_BITS);

				glDisable(GL_LIGHTING);
				glLineWidth(3.0f);
				glColor4fv(color);

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

				glPopAttrib();
				glPopMatrix();
			}
		};

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