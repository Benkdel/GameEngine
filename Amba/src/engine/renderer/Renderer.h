#pragma once

#include <engine/ResourceManager.h>
#include <engine/io/Camera.h>

#include <engine/ecs/Scene.h>

namespace Amba {

	class Renderer
	{
	public:
		static void BeginScene(Camera& camera, Scene* scene);
		static void EndScene();

		static void Draw(const VertexArray *va, const IndexBuffer *ib, Shader *shader, const glm::mat4& perspective, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawMesh(const Mesh* mesh, const VertexBufferLayout& layout, Shader* shader, const glm::mat4& perspective, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawTriangles(const VertexArray* va, unsigned int count, Shader* shader, const glm::mat4& perspective, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawModel(const Model* model, const VertexBufferLayout& layout, Shader* shader, const glm::mat4& perspective, const glm::mat4& transform = glm::mat4(1.0f));
		
		static void DrawActiveScene(Shader *shader, const glm::mat4& perspective);

#ifdef AB_ENABLE_ASSERTS
		static void DrawGrid(Scene* scene, const glm::mat4& perspective, const glm::mat4& transform = glm::mat4(1.0f));
#endif
		
		static void Clear(glm::vec4 clearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
	private:
		struct SceneData
		{
			Scene* m_Scene;
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;
		
		friend class Model;
		friend class Interface;
	};
}

// TODO
/*
*	- later on create an abstract class and then derive for each API (vulkan, opengl etc)
	- CREATE A SUBMISION BUFFER
	- DO AUTOMIZATIONS
	- THEN RENDER THE SCENE

	- RENDER MULTITHREAD WISE
*/