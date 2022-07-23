#pragma once

#include <engine/ResourceManager.h>
#include <engine/io/Camera.h>

namespace Amba {

	class Renderer
	{
	public:
		static void BeginScene(Camera& camera);
		static void EndScene();

		static void Draw(const VertexArray *va, const IndexBuffer *ib, Shader *shader, const glm::mat4 perspective, const glm::mat4& transform = glm::mat4(1.0f));
		static void DrawModel(const Model* model, const VertexBufferLayout& layout, Shader* shader, const glm::mat4 perspective, const glm::mat4& transform = glm::mat4(1.0f));

		static void Clear();
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static SceneData* m_SceneData;

		friend class Model;
	};
}
