# pragma once

#include <engine/ecs/Scene.h>

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

	class Primitives
	{
	public:

		Primitives() {}

		void Destroy()
		{
			p_Scene = nullptr;
		}

		~Primitives() 
		{
			p_Scene = nullptr;
		}

		EntityId m_EntityId;
	
	protected:
		virtual void Init() {};
		Scene* p_Scene;

	private:

	};

	class Cube : public Primitives
	{
	public:

		Cube(Scene* scene)
		{
			p_Scene = scene;
			m_EntityId = scene->CreateEntity();

			Init();
		}

	private:

		void Init()
		{
			p_Scene->AddComponent<MeshComponent>(m_EntityId);
			p_Scene->AddComponent<TransformComponent>(m_EntityId);

			MeshComponent* mesh = p_Scene->GetComponent<MeshComponent>(m_EntityId);

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
	};

	class Sphere : public Primitives
	{
	public:


	private:

	};

}