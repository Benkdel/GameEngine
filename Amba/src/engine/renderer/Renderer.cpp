#include "Renderer.h"

namespace Amba {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(Camera* camera, Scene* scene)
	{
		m_SceneData->m_Scene = scene;
		m_SceneData->ViewProjectionMatrix = camera->GetViewMatrix();
	}

	void Renderer::EndScene()
	{
		m_SceneData->m_Scene->Cleanup();
		m_SceneData->m_Scene = nullptr;
	}

	void Renderer::Draw(const VertexArray* va, const IndexBuffer* ib, Shader* shader, const glm::mat4& perspective, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniform4mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix); // this should not be called every frame?
		shader->SetUniform4mat("u_Perspective", perspective);
		shader->SetUniform4mat("u_Transform", transform);
		va->Bind();
		ib->Bind();
		glDrawElements(GL_TRIANGLES, ib->GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::DrawTriangles(const VertexArray* va, unsigned int count, Shader* shader, const glm::mat4& perspective, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniform4mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetUniform4mat("u_Perspective", perspective);
		shader->SetUniform4mat("u_Transform", transform);
		va->Bind();
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void Renderer::DrawActiveScene(Shader* shader, const glm::mat4& perspective)
	{
		// need to optimize sorting entities by shader so I minimize bind / unbind
		Shader* sh;

		for (EntityId ent : EntityGroup<MeshComponent>(m_SceneData->m_Scene->p_EntHandler))
		{
			MeshComponent *mesh = m_SceneData->m_Scene->GetComponent<MeshComponent>(ent);
			TransformComponent* trs = m_SceneData->m_Scene->GetComponent<TransformComponent>(ent);

			if (mesh->p_Shader != nullptr)
				sh = mesh->p_Shader;
			else
				sh = shader;

			sh->Bind();
			sh->SetUniform4mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix); // should this be called every frame?
			sh->SetUniform4mat("u_Perspective", perspective);

			AB_ASSERT(!mesh->m_Indices.empty(), "Error in renderer function: DrawMeshes. Indices vector is empty!");
			AB_ASSERT(!mesh->m_Vertices.empty(), "Error in renderer function: DrawMeshes. Vertices vector is empty!");

			glm::mat4 transform = trs->GetTrsMatrix();
			transform = glm::translate(transform, trs->GetPosition());
			transform = glm::scale(transform, trs->GetScale());
			transform = glm::rotate(transform, trs->GetRotAngle(), trs->GetRotationAxis());
			sh->SetUniform4mat("u_Transform", transform);

			VertexArray va;
			VertexBuffer vbo(&mesh->m_Vertices[0], (unsigned int)mesh->m_Vertices.size() * sizeof(Vertex));

			va.AddBuffer(&vbo, mesh->layout);
			IndexBuffer ib(&mesh->m_Indices[0], (unsigned int)mesh->m_Indices.size());

			if (mesh->m_Textures.size() > 0)
			{
				// fow now only using 1 texture
				mesh->m_Textures[0]->Bind();
				sh->SetUniform1i("u_Texture", 0);
			}

			va.Bind();
			ib.Bind();

			glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

		}
		
	}

	void Renderer::Clear(glm::vec4 clearColor)
	{
		glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}
