#ifndef __GPhysics_hpp__
#define __GPhysics_hpp__
#include <glm/glm.hpp>
#include <btBulletDynamicsCommon.h>
namespace g4y
{
	glm::vec3 convert_btvec3_to_vec3(const btVector3& btVec3);
	btVector3 convert_vec3_to_btvec3(const glm::vec3& vec3);
}
#endif
