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

		for (auto& mesh : model->m_Meshes)
		{
			// compute transforms per mesh
			
			// calculate transforms from model loading
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), mesh.m_TSR.scaling);
			glm::mat4 modelTransform = glm::translate(glm::mat4(1.0f), mesh.m_TSR.translation) * scale;
			//modelTransform = glm::rotate(transform, mesh.m_TSR.rotation.angle, mesh.m_TSR.rotation.vec3);

			// add on top transforms (how should I do this?)
			// something like, modelTransfor += transform

			// set shader transform
			shader->SetUniform4mat("u_Transform", modelTransform); // for now, i dont know where to put transforms

			if (mesh.ContainsTextures())
			{
				unsigned int diffuseIdx = 0;
				unsigned int specularIdx = 0;

				for (unsigned int i = 0; i < mesh.m_Textures.size(); i++)
				{
					glActiveTexture(GL_TEXTURE0 + i);

					// update this
					std::string name;
					switch (mesh.m_Textures[i].m_TexType)
					{
					case 0:
						name = "diffuse" + std::to_string(diffuseIdx++);
						break;
					case 1:
						name = "specular" + std::to_string(specularIdx++);
						break;
					default:
						break;
					}
					
					// set the shader value
					shader->SetUniform1i(name, i);
					mesh.m_Textures[i].Bind();
				}
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
