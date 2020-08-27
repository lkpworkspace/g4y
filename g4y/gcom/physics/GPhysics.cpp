#include "GPhysics.hpp"
namespace g4y
{
	glm::vec3 convert_btvec3_to_vec3(const btVector3& btVec3)
	{
		return glm::vec3(btVec3.x(), btVec3.y(), btVec3.z());
	}
	btVector3 convert_vec3_to_btvec3(const glm::vec3& vec3)
	{
		return btVector3(vec3.x, vec3.y, vec3.z);
	}
}

