#define FROST_EXPORT
#include "MovableObject3D.h"

namespace Frost
{
	namespace Graphics3D
	{
		MovableObject3D::MovableObject3D(void)
		{
		}

		MovableObject3D::~MovableObject3D(void)
		{
		}

		Vector3f MovableObject3D::getLocation()
		{
			return Location;
		}

		void MovableObject3D::setLocation(Vector3f &vec)
		{
			Location = vec;
		}

		void MovableObject3D::setLocation(float x, float y, float z)
		{
			Location = Vector3f(x, y, z);
		}

		void MovableObject3D::Scale(Vector3f &scl)
		{
			//
		}

		void MovableObject3D::Scale(float x, float y, float z)
		{
			//
		}

		void MovableObject3D::Translate(float x, float y, float z)
		{
			Location += Vector3f(x, y, z);
		}

		void MovableObject3D::Translate(Vector3f &mov)
		{
			Location += mov;
		}

		void MovableObject3D::Roll(float Angle)
		{
			Rotations.z = Angle;
		}

		void MovableObject3D::Pitch(float Angle)
		{
			Rotations.x = Angle;
		}

		void MovableObject3D::Yaw(float Angle)
		{
			Rotations.y = Angle;
		}

		float MovableObject3D::getRoll()
		{
			return Rotations.z;
		}

		float MovableObject3D::getPitch()
		{
			return Rotations.x;
		}

		float MovableObject3D::getYaw()
		{
			return Rotations.y;
		}

		MovableObject3D &MovableObject3D::operator + (MovableObject3D &mo3d)
		{
			MovableObject3D ret((*this));
			ret.Location += mo3d.Location;
			ret.Rotations += mo3d.Rotations;
			return ret;
		}
	};
};