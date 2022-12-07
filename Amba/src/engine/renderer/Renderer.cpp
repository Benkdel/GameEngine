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

#ifdef AB_ENABLE_ASSERTS
	void Renderer::DrawGrid(Scene* scene, const glm::mat4& perspective, const glm::mat4& transform)
	{
		Shader shader("src/engine/res/shaders/gridVS.glsl", "src/engine/res/shaders/gridFS.glsl");

		shader.Bind();
		shader.SetUniform4mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix); // this should not be called every frame?
		shader.SetUniform4mat("u_Perspective", perspective);
		shader.SetUniform4mat("u_Transform", glm::mat4(1.0f));

		shader.SetUniform4f("u_Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		Spatial2DGrid grid = *scene->m_Spatial2DGrid;

		Amba::VertexArray va;
		Amba::VertexBuffer vbo(&grid.m_Vertices[0], (unsigned int)grid.m_Vertices.size() * sizeof(glm::vec3));

		Amba::VertexBufferLayout layout;
		layout.Push<float>(3);
		
		va.AddBuffer(&vbo, layout);
		IndexBuffer ib(&grid.m_Indices[0], (unsigned int)grid.m_Indices.size());
		
		va.Bind();
		ib.Bind();

		glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
#endif

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

		for (EntityId ent : SceneView<MeshComponent>(m_SceneData->m_Scene))
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
			transform = glm::rotate(transform, trs->m_Radians, trs->GetRotationAxis());
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
