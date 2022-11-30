# pragma once

#include <engine/dataTypes.h>
#include <engine/ecs/Entcs.h>

static float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

namespace Amba {

	class Primitives : public Entity
	{
	public:

		Primitives(Scene* scene)
			: Entity(scene) {}
	
		virtual void Init() {};

	private:

	};

	class Cube : public Primitives
	{
	public:

		Cube(Scene* scene)
			: Primitives(scene) {}

		void Init()
		{
			AddComponent<MeshComponent>();
			AddComponent<TransformComponent>();
			
			MeshComponent* mesh = GetComponent<MeshComponent>();

			// lets see if we can change this to take a material instead
			// how to handle when I want a default material, no material, etc.
			for (int i = 0; i < 36 * 5; i += 5)
			{
				glm::vec3 positions = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
				glm::vec2 texCoords = glm::vec2(vertices[i + 3], vertices[i + 4]);
				mesh->m_Vertices.push_back({ positions, glm::vec3(0.0, 1.0f, 0.0f), texCoords});
			}

			for (int i = 0; i < 36; i++)
				mesh->m_Indices.push_back(i);

			Amba::VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);

			mesh->layout = layout;

			mesh = nullptr;
		}
	
	private:

	};

	class Sphere : public Primitives
	{
	public:
		Sphere(Scene* scene, float radius, int stacks, int sectors) 
			: Primitives(scene)
			{
				m_Radius = radius;
				m_Stacks = stacks;
				m_Sectors = sectors;
				m_Center = glm::vec3(0.0f);
			}

		float m_Radius;
		glm::vec3 m_Center;

		void Init()
		{
			AddComponent<MeshComponent>();
			AddComponent<TransformComponent>();

			MeshComponent* mesh = GetComponent<MeshComponent>();

			float x, y, z, xy;									// vertex position
			float nx, ny, nz, lengthInv = 1.0f / m_Radius;		// normal
			float s, t;											// texCoord

			float sectorStep = 2 * PI_VALUE / m_Sectors;
			float stackStep = PI_VALUE / m_Stacks;
			float sectorAngle, stackAngle;

			for (int i = 0; i <= m_Stacks; ++i)
			{
				stackAngle = PI_VALUE / 2 - i * stackStep;        // starting from pi/2 to -pi/2
				xy = m_Radius * cosf(stackAngle);             // r * cos(u)
				z = m_Radius * sinf(stackAngle);              // r * sin(u)

				// add (sectorCount+1) vertices per stack
				// the first and last vertices have same position and normal, but different tex coords
				for (int j = 0; j <= m_Sectors; ++j)
				{
					sectorAngle = j * sectorStep;           // starting from 0 to 2pi

					// vertex position
					x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
					y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
					glm::vec3 position(x, y, z);

					// normalized vertex normal
					nx = x * lengthInv;
					ny = y * lengthInv;
					nz = z * lengthInv;
					glm::vec3 normals(nx, ny, nz);

					// vertex tex coord between [0, 1]
					s = (float)j / m_Sectors;
					t = (float)i / m_Stacks;
					glm::vec2 texCoords(s, t);

					mesh->m_Vertices.push_back({ position, normals, texCoords });
				}
			}

			// indices
			//  k1--k1+1
			//  |  / |
			//  | /  |
			//  k2--k2+1
			unsigned int k1, k2;
			for (int i = 0; i < m_Stacks; ++i)
			{
				k1 = i * (m_Sectors + 1);     // beginning of current stack
				k2 = k1 + m_Sectors + 1;      // beginning of next stack

				for (int j = 0; j < m_Sectors; ++j, ++k1, ++k2)
				{
					// 2 triangles per sector excluding 1st and last stacks
					if (i != 0)
					{
						mesh->m_Indices.push_back(k1);
						mesh->m_Indices.push_back(k2);
						mesh->m_Indices.push_back(k1 + 1);		// k1---k2---k1+1
					}

					if (i != (m_Stacks - 1))
					{
						mesh->m_Indices.push_back(k1 + 1);
						mesh->m_Indices.push_back(k2);
						mesh->m_Indices.push_back(k2 + 1);		// k1+1---k2---k2+1
					}
				}
			}
			
			Amba::VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);

			mesh->layout = layout;

			mesh = nullptr;
		}

		private:
			int m_Stacks;
			int m_Sectors;
	};


	// work in progress
	class Plane : public Primitives
	{
	public:
		Plane(Scene* scene, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, unsigned int div)
			: m_V0(v0), m_V1(v1), m_V2(v2), m_V3(v3), m_Division(div), Primitives(scene)
		{
		}

		void Init()
		{
			AddComponent<MeshComponent>();
			AddComponent<TransformComponent>();

			MeshComponent* mesh = GetComponent<MeshComponent>();

			float factor = 1 / (float)m_Division;

			glm::vec3 vec03 = (m_V3 - m_V0) * factor;
			glm::vec3 vec12 = (m_V2 - m_V1) * factor;

			for (unsigned int row = 0; row < m_Division + 1; row++)
			{
				glm::vec3 start = m_V0 + vec03 * (float)row;
				glm::vec3 end = m_V1 + vec12 * (float)row;
				glm::vec3 vecDiv = (end - start) * factor;

				for (unsigned int col = 0; col < m_Division + 1; col++)
				{
					glm::vec3 pos = start + vecDiv * (float)col;

					// calculate normals from tangents?
					glm::vec3 tangent = glm::normalize(vecDiv);
					glm::vec3 nextStart = start + vec03;
					glm::vec3 nextEnd = end + vec12;
					glm::vec3 nextVecDiv = (nextEnd - nextStart) * factor;
					glm::vec3 nextRowPos = nextStart + nextVecDiv * (float)col;
					glm::vec3 possibleBiTangent = glm::normalize(nextRowPos - pos);

					glm::vec3 normal = glm::cross(tangent, possibleBiTangent);

					glm::vec2 texCoords = glm::vec2((float)col * factor, (float)row * factor);

					mesh->m_Vertices.push_back({ pos, normal, texCoords });
				}
			}

			// indices
			for (unsigned int row = 0; row < m_Division; row++)
			{
				for (unsigned int col = 0; col < m_Division; col++)
				{
					unsigned int index = row * (m_Division + 1) + col;

					// top triangle
					mesh->m_Indices.push_back(index);
					mesh->m_Indices.push_back(index + (m_Division + 1) + 1);
					mesh->m_Indices.push_back(index + (m_Division + 1));

					// bottom triangle
					mesh->m_Indices.push_back(index);
					mesh->m_Indices.push_back(index + 1);
					mesh->m_Indices.push_back(index + (m_Division + 1) + 1);
				}
			}

			Amba::VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);

			mesh->layout = layout;

			mesh = nullptr;
		}

	private:
		
		glm::vec3 m_V0 = glm::vec3(0.0f);
		glm::vec3 m_V1 = glm::vec3(0.0f);
		glm::vec3 m_V2 = glm::vec3(0.0f);
		glm::vec3 m_V3 = glm::vec3(0.0f);
		unsigned int m_Division = 0;


	};

}