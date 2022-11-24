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
		Plane(Scene* scene, glm::vec3 normal, float length, float width)
			: m_Normal(glm::normalize(normal)), m_Length(length), m_Width(width), Primitives(scene)
		{
		}

		void Init()
		{
			AddComponent<MeshComponent>();
			AddComponent<TransformComponent>();

			MeshComponent* mesh = GetComponent<MeshComponent>();

			// rendering a plane using normal

			// (use width and length for x and z, solve for y)
			float x0 = -m_Length / 2.0f;
			float x1 = -x0;
			float z0 = -m_Width / 2.0f;
			float z1 = -z0;

			glm::vec3 positions[4] = {
				{x0, 0.0f, z0},
				{x0, 0.0f, z1},
				{x1, 0.0f, z1},
				{x1, 0.0f, z0}
			};

			glm::vec2 texCoords[4] = {
				{0.0f, 0.0f},
				{1.0f, 0.0f},
				{0.0f, 1.0f},
				{1.0f, 1.0f}
			};

			// solve for y values and populate mesh
			// then we could add the option to have many more vertices, for now just corners
			int row = 0;
			int col = 0;
			int numQuads = 1;
			int numIterations = numQuads * 4 / 2;
			
			for (int i = 0; i < numIterations * 2; i++)
			{
				positions[i].y = (m_Normal.x / m_Normal.y * (positions[i].x - x0) -
					m_Normal.z / m_Normal.y * (positions[i].z - z0) + 0.0f);
			
				glm::vec2 texCoords = glm::vec2(0.0f);
				switch (i % 4)
				{
				case 0: break;
				case 1: texCoords = glm::vec2(1.0f, 0.0f); break;
				case 2: texCoords = glm::vec2(1.0f, 1.0f); break;
				case 3: texCoords = glm::vec2(0.0f, 1.0f); break;
				default: break;
				}

				mesh->m_Vertices.push_back({ positions[i], m_Normal, texCoords });
			}

			// only for now:

			mesh->m_Indices.push_back(0);
			mesh->m_Indices.push_back(1);
			mesh->m_Indices.push_back(2);
			mesh->m_Indices.push_back(0);
			mesh->m_Indices.push_back(2);
			mesh->m_Indices.push_back(1);

			Amba::VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);

			mesh->layout = layout;

			mesh = nullptr;
		}

	private:
		glm::vec3 m_Normal = glm::vec3(0.0f);
		float m_Length = 0.0f;
		float m_Width = 0.0f;

	};

}