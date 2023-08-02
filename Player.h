#pragma once

#include "typedefs.h"
#include "WorldObject.h"
#include "ScreenLogger.h"


namespace engine
{
	class Player : public WorldObject
	{
	public:
		static Player& getInstance() 
		{
			static Player instance;
			return instance;
		}

		WorldObject& createCollisionBox(float x, float y, float z) override
		{
			auto* AABB = new CollisionBox;

			AABB->x1 = -0.5f;
			AABB->x2 = +0.5f;

			AABB->y1 = 0.05f;
			AABB->y2 = 2.0f;

			AABB->z1 = -0.5f;
			AABB->z2 = +0.5f;

			CBlist.push_back(AABB);

			return (*this);
		}

		Player& updateCollisionBox()
		{
			for (auto bb : CBlist) 
			{
				bb->x1 = (float)cam_pos[0] - 0.5f;
				bb->x2 = (float)cam_pos[0] + 0.5f;

				bb->y1 = (float)cam_pos[1] - 1.65f;
				bb->y2 = (float)cam_pos[1] + 0.3f;

				bb->z1 = (float)cam_pos[2] - 0.5f;
				bb->z2 = (float)cam_pos[2] + 0.5f;

				ScreenLogger::getInstance().logMessage(
					"AABB: from {%.3lf, %.3lf, %.3lf} to {%.3lf, %.3lf, %.3lf}",
					bb->x1, bb->y1, bb->z1, bb->x2, bb->y2, bb->z2
				);
			}
			
			return (*this);
		}


	private:
		Player() : 
			WorldObject(),
			move_speed(20.0), 
			cam_dir{ 0.0, 0.0, -1.0 }, 
			cam_pos{ 0.0, 1.7, 0.0 }, 
			torso_dir{ 0.0, 0.0, -1.0 }, 
			left_dir{ -1.0, 0.0, 0.0 }
		{}

	public:
		double move_speed;

		volatile vector3d cam_dir;
		volatile vector3d cam_pos;
		volatile vector3d torso_dir;
		volatile vector3d left_dir;
	};
}