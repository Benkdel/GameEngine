#include "Renderer.h"

namespace Amba {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::BeginScene(Camera& camera, Scene* scene)
	{
		m_SceneData->m_Scene = scene;
		m_SceneData->ViewProjectionMatrix = camera.GetViewMatrix();
	}

	void Renderer::EndScene()
	{
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

		Spatial2DGrid& grid = scene->m_Spatial2DGrid;

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

	void Renderer::DrawMesh(const Mesh* mesh, const VertexBufferLayout& layout, Shader* shader, const glm::mat4& perspective, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniform4mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix); // this should not be called every frame?
		shader->SetUniform4mat("u_Perspective", perspective);
		//shader->SetUniform4mat("u_Transform", transform);

		glm::mat4 tsr = glm::scale(glm::mat4(1.0f), glm::vec3(mesh->m_Size));
		tsr = glm::translate(tsr, mesh->m_Translation);
		shader->SetUniform4mat("u_Transform", tsr);

		VertexArray va;
		VertexBuffer vbo(&mesh->m_Vertices[0], (unsigned int)mesh->m_Vertices.size() * sizeof(Vertex));

		va.AddBuffer(&vbo, layout);
		IndexBuffer ib(&mesh->m_Indices[0], (unsigned int)mesh->m_Indices.size());

		va.Bind();
		ib.Bind();

		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
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

	void Renderer::DrawModel(const Model* model, const VertexBufferLayout& layout, Shader* shader, const glm::mat4& perspective, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetUniform4mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		shader->SetUniform4mat("u_Perspective", perspective);

		int id = 0;
 		for (auto& mesh : model->m_Meshes)
		{
			glm::mat4 finalTSR = glm::scale(mesh.m_TSR, glm::vec3(model->m_Size));
			finalTSR = glm::translate(finalTSR, model->m_Translation);
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

	void Renderer::DrawActiveScene(Shader* shader, const glm::mat4& perspective)
	{
		shader->Bind();
		shader->SetUniform4mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix); // should this be called every frame?
		shader->SetUniform4mat("u_Perspective", perspective);

		for (EntityId ent : SceneView<MeshComponent, TransformComponent>(m_SceneData->m_Scene))
		{
			MeshComponent *mesh = m_SceneData->m_Scene->GetComponent<MeshComponent>(ent);
			TransformComponent* trs = m_SceneData->m_Scene->GetComponent<TransformComponent>(ent);

			AB_ASSERT(!mesh->m_Indices.empty(), "Error in renderer function: DrawMeshes. Indices vector is empty!");
			AB_ASSERT(!mesh->m_Vertices.empty(), "Error in renderer function: DrawMeshes. Vertices vector is empty!");

			glm::mat4 tsr = glm::mat4(1.0f);
			tsr = glm::translate(tsr, trs->m_Position);
			tsr = glm::scale(tsr, glm::vec3(trs->m_Size));
			shader->SetUniform4mat("u_Transform", tsr);

			VertexArray va;
			VertexBuffer vbo(&mesh->m_Vertices[0], (unsigned int)mesh->m_Vertices.size() * sizeof(Vertex));

			va.AddBuffer(&vbo, mesh->layout);
			IndexBuffer ib(&mesh->m_Indices[0], (unsigned int)mesh->m_Indices.size());

			mesh->p_Texture->Bind();
			shader->SetUniform1i("u_Texture", 0);

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
