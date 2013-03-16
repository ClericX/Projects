#pragma once

#include "DLL.h"

#include "Vector3f.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI MovableObject3D
		{
		public:
			MovableObject3D(void);
			~MovableObject3D(void);

			Vector3f getLocation();
			void setLocation(Vector3f &vec);
			void setLocation(float x, float y, float z);
			void Scale(Vector3f &scl);
			void Scale(float x, float y, float z);
			void Translate(float x, float y, float z);
			void Translate(Vector3f &mov);
			void Roll(float Angle); // Rotate around the Z-axis
			void Pitch(float Angle); // Rotate around the X-axis
			void Yaw(float Angle); // Rotate around the Y-axis
			float getRoll();
			float getPitch();
			float getYaw();
			MovableObject3D &operator + (MovableObject3D &mo3d);

		private:
			Vector3f Location;
			Vector3f Rotations;
		};
	};
};