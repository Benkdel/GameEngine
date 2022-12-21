#pragma once

// TODO: change file name to something more meaninfull

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// types
typedef unsigned long long EntityId;
typedef unsigned int EntityIndex;
typedef unsigned int EntityVersion;


// definitions
#define BOX_COLLIDER_SIZE 5.0f
#define PI_VALUE 3.141592653589793

#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.f

// only for colliders
class IntersectData
{
public:

	IntersectData(const bool doesIntersect, const glm::vec3 direction, const glm::vec3 collisionNorm, const unsigned long long otherEnt = -1) :
		m_DoesIntersect(doesIntersect), m_Direction(direction), m_CollisionNorm(collisionNorm), m_OtherEnt(otherEnt) {}

	inline bool GetDoesIntersect()			const { return m_DoesIntersect; };
	inline float GetDistance()				const { return glm::length(m_Direction); };
	inline glm::vec3 GetDirection()			const { return m_Direction; };
	inline glm::vec3 GetCollisionNorm()		const { return m_CollisionNorm; };
	inline unsigned long long GetOtherEnt()			const { return m_OtherEnt; };

private:

	const bool m_DoesIntersect				= false;
	const glm::vec3 m_Direction				= glm::vec3(0.0f);
	const glm::vec3 m_CollisionNorm			= glm::vec3(0.0f);
	const unsigned long long m_OtherEnt		= -1;
};

namespace Amba {

	static float s_InfiniteMassValue = 1000000.0f;

	struct ViewPortData
	{
		unsigned int m_Width;
		unsigned int m_Height;
		bool m_HasChanged = false;
	};

	struct Rotation {
		float angle;
		glm::vec3 vec3;
	};

	struct Vertex {
		glm::vec3 v_Position;
		glm::vec3 v_Normals;
		glm::vec2 v_TexCoords;
	};
}

