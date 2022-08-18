#include "Renderer.h"

namespace Amba {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewMatrix();
		

	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Draw(const VertexArray* va, const IndexBuffer* ib, Shader* shader, const glm::mat4 perspective, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniform4mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix); // this should not be called every frame?
		shader->SetUniform4mat("u_Perspective", perspective);
		shader->SetUniform4mat("u_Transform", transform);
		va->Bind();
		ib->Bind();
		glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawModel(const Model* model, const VertexBufferLayout& layout, Shader* shader, const glm::mat4 perspective, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniform4mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetUniform4mat("u_Perspective", perspective);

		int id = 0;
 		for (auto& mesh : model->m_Meshes)
		{
			glm::mat4 finalTSR = glm::scale(mesh.m_TSR, glm::vec3(0.1f));
			shader->SetUniform4mat("u_Transform", finalTSR);

			// for now, I will use models with only 1 texture for each mesh
			if (mesh.ContainsTextures())
			{
				std::string name = "u_Texture";
				
				// set the shader value
				shader->SetUniform1i(name, id);
				mesh.m_Textures[0].Bind(id++);
			}

			VertexArray va;
			VertexBuffer vbo(&mesh.m_Vertices[0], (unsigned int)mesh.m_Vertices.size() * sizeof(Vertex));
			
			va.AddBuffer(&vbo, layout);
			IndexBuffer ib(&mesh.m_Indices[0], (unsigned int)mesh.m_Indices.size());
			
			va.Bind();
			ib.Bind();
			
			glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
		}
	}

	void Renderer::Clear()
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}
