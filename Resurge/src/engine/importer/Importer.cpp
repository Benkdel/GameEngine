#include "Importer.h"

namespace ABImp {

	Importer::Importer()
	{
	}

	void Importer::LoadData(const std::string & filePath)
	{
		std::ifstream input(filePath);
		AB_ASSERT(input.is_open(), ("Failed to open model at {0}", filePath));
		m_Data = json::parse(input);
		input.close();

		m_Directory = filePath.substr(0, filePath.find_last_of('/') + 1);

		std::string uri = m_Data["buffers"][0]["uri"];							// for now im assuming that only 1 buffer exists, just for testing purposes
		std::string path = m_Directory + uri;

		/* LOAD STRUCTS */
		GetMeshesMetadata();
		GetAccessorsMetadata();
		GetBufferViewsMetadata();
		GetBuffersMetadata();

		/* PROCESS DATA INTO OPENGL FORMAT */
		GetBinData();
		GetMeshData();
		GetNodes();
		GetMaterials();
	}

	void Importer::Cleanup()													// Clean all data
	{
		m_Data.clear();
		m_MD_Meshes.clear();
		m_MD_Accessors.clear();
		m_MD_BufferViews.clear();
		m_MD_Buffers.clear();
		m_Directory = "";
		m_BinData.clear();
	}

	void Importer::GetMeshesMetadata()
	{
		for (auto& mesh : m_Data["meshes"])
		{
			std::string name = mesh.value("name", "null");
			json primitives = mesh["primitives"][0];
			json attributes = primitives["attributes"];

			unsigned int position = attributes.value("POSITION", -1);
			unsigned int normals = attributes.value("NORMAL", -1);
			unsigned int texcoords = attributes.value("TEXCOORD_0", -1);
			impAttributesIndexes att = { position, normals, texcoords };

			unsigned int indices = primitives.value("indices", -1);
			unsigned int material = primitives.value("material", -1);
			unsigned int mode = primitives.value("mode", -1);

			// push back mesh indexes struct 
			m_MD_Meshes.push_back({ name, att, indices, material, mode });
		}
	}

	void Importer::GetAccessorsMetadata()
	{
		for (auto& accesor : m_Data["accessors"])
		{
			unsigned int bufferView = accesor["bufferView"];
			unsigned int byteOffset = accesor.value("byteOffset", 0);
			unsigned int componentType = accesor["componentType"];
			unsigned int count = accesor["count"];
			std::string type = accesor["type"];
			unsigned int size = GetNumOfElements(type) * GetSizeOfComponent(componentType);

			m_MD_Accessors.push_back({ bufferView, byteOffset, componentType, count, type, size });
		}
	}

	void Importer::GetBufferViewsMetadata()
	{
		for (auto& bufferView : m_Data["bufferViews"])
		{
			unsigned int buffer = bufferView["buffer"];
			unsigned int byteLength = bufferView["byteLength"];
			unsigned int byteOffset = bufferView.value("byteOffset", 0);
			unsigned int byteStride = bufferView.value("byteStride", 0);
			std::string name = bufferView.value("name", "null");
			unsigned int target = bufferView.value("target", -1);

			m_MD_BufferViews.push_back({ buffer, byteLength, byteOffset, byteStride, name, target });
		}
	}

	void Importer::GetBuffersMetadata()
	{
		for (auto& buffer : m_Data["buffers"])
		{
			unsigned int byteLength = buffer["byteLength"];
			std::string uri = buffer["uri"];

			m_MD_Buffers.push_back({ byteLength, uri });
		}
	}

	/*
	=================================
	Translate data into OPENGL format
	=================================
	*/

	void Importer::GetBinData()
	{
		for (auto& buffer : m_MD_Buffers)
		{
			std::string path = m_Directory + buffer.uri;
			std::ifstream input(path, std::ios::binary);

			AB_ASSERT(input.is_open(), ("Failed to open bin file at {0}", path));

			std::stringstream ss;
			ss << input.rdbuf();
			std::string bytesText = ss.str();

			std::vector<unsigned char> bytes(bytesText.begin(), bytesText.end());
			m_BinData.push_back(bytes);
		}
	}

	// get meshes before nodes
	void Importer::GetMeshData()
	{
		for (unsigned int i = 0; i < m_MD_Meshes.size(); i++)
		{
			/* loop through meshes */
			impMeshIndexes meshMD = m_MD_Meshes.at(i);
			Mesh mesh;
			std::vector<Vertex> vertices;

			/* every mesh has primitives used by accesors */
			if (meshMD.indices != -1)
			{
				impAccesor accesor = m_MD_Accessors.at(meshMD.indices);
				impBufferView bufferView = m_MD_BufferViews.at(accesor.bufferView);
				std::vector<unsigned char> data = m_BinData.at(bufferView.buffer);
				const unsigned int beginningOfData = bufferView.byteOffset + accesor.byteOffset;
				const unsigned int lenghtOfData = accesor.count * GetSizeOfComponent(accesor.componentType) * GetNumOfElements(accesor.type);

				for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i)
				{
					unsigned int value = 0;
					ConvertBytes<unsigned int>(value, data, accesor, i);
 					mesh.indices.push_back((unsigned int)value);
				}
			}

			if (meshMD.attributes.Positions != -1)
			{
				impAccesor accesor = m_MD_Accessors.at(meshMD.attributes.Positions);
				impBufferView bufferView = m_MD_BufferViews.at(accesor.bufferView);
				std::vector<unsigned char> data = m_BinData.at(bufferView.buffer);
				const unsigned int beginningOfData = bufferView.byteOffset + accesor.byteOffset;
				const unsigned int lenghtOfData = accesor.count * GetSizeOfComponent(accesor.componentType) * GetNumOfElements(accesor.type);

				unsigned int ctr = 0;
				float pos[3];

				for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i)
				{
					float value = 0.0f;
					ConvertBytes<float>(value, data, accesor, i);
					pos[ctr++] = value;

					if (ctr == 3)
					{
						glm::vec3 position = { pos[0], pos[1], pos[2] };
						glm::vec3 normPlaceHolder = { 0.0f, 0.0f, 0.0f };
						glm::vec2 texPlaceHolder = { 0.0f, 0.0f };
						vertices.push_back({ position, normPlaceHolder, texPlaceHolder });
						ctr = 0;
					}					
				}
			}

			if (meshMD.attributes.Normals != -1)
			{
				impAccesor accesor = m_MD_Accessors.at(meshMD.attributes.Normals);
				impBufferView bufferView = m_MD_BufferViews.at(accesor.bufferView);
				std::vector<unsigned char> data = m_BinData.at(bufferView.buffer);
				const unsigned int beginningOfData = bufferView.byteOffset + accesor.byteOffset;
				const unsigned int lenghtOfData = accesor.count * GetSizeOfComponent(accesor.componentType) * GetNumOfElements(accesor.type);

				unsigned int ctr = 0;
				unsigned int idx = 0;
				float pos[3];
				for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i)
				{
					float value = 0.0f;
					ConvertBytes<float>(value, data, accesor, i);
					pos[ctr++] = value;

					if (ctr == 3)
					{
						glm::vec3 norms = { pos[0], pos[1], pos[2] };
						vertices.at(idx++).v_Normals = norms;
						ctr = 0;
					}
				}
			}

			if (meshMD.attributes.Texcoords != -1)
			{
				impAccesor accesor = m_MD_Accessors.at(meshMD.attributes.Texcoords);
				impBufferView bufferView = m_MD_BufferViews.at(accesor.bufferView);
				std::vector<unsigned char> data = m_BinData.at(bufferView.buffer);
				const unsigned int beginningOfData = bufferView.byteOffset + accesor.byteOffset;
				const unsigned int lenghtOfData = accesor.count * GetSizeOfComponent(accesor.componentType) * GetNumOfElements(accesor.type);

				unsigned int ctr = 0;
				unsigned int idx = 0;
				float pos[2];
				for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i)
				{
					float value = 0.0f;
					ConvertBytes<float>(value, data, accesor, i);
					pos[ctr++] = value;

					if (ctr == 2)
					{
						glm::vec2 tex = { pos[0], pos[1] };
						vertices.at(idx++).v_TexCoords = tex;
						ctr = 0;
					}
				}
			}

			mesh.vertices = vertices;
			m_Meshes.push_back(mesh);
		}
	}

	void Importer::GetNodes()
	{
		for (auto& node : m_Data["nodes"])
		{
			std::vector<int> meshes;
			std::vector<int> children;

			std::cout << node << "\n";

			std::string name = node.value("name", "null");
			
			// Add index of nodes's meshes
			if (node.contains("mesh"))
			{
				for (auto& mesh : node["mesh"])
					meshes.push_back(mesh);
			}
			
			// Add index of node's children
			if (node.contains("children"))
			{
				for (auto& child : node["children"])
				{
					children.push_back(child);
				}
			}

			/* 
			===========
			SET TSR 
			===========
			*/
			glm::mat4 tsr = glm::mat4(1.0f);

			// check if there are other potential names for tsr matrix in gltf files
			if (node.contains("matrix"))
			{
				tsr = (glm::mat3)node["matrix"][0];
			}
			else
			{
				// build matrix from vectors
				if (node.contains("translation"))
				{
					tsr = glm::translate(tsr,
										glm::vec3(
											node["translation"][0],
											node["translation"][1],
											node["translation"][2]
						));
				}

				if (node.contains("scale"))
				{
					tsr = glm::scale(tsr,
									glm::vec3(
										node["scale"][0],
										node["scale"][1],
										node["scale"][2]
					));
				}

				if (node.contains("rotation"))
				{
					float angle = node["rotation"][0];
					tsr = glm::rotate(tsr, angle, 
									glm::vec3(
										node["rotation"][1],
										node["rotation"][2],
										node["rotation"][3]
					));
				}
			}

			// push node data
			m_Nodes.push_back({ name, (unsigned int)meshes.size(), (unsigned int)children.size(), meshes, children, tsr });
		}
	}

	void Importer::GetMaterials()
	{
		// todo materials / textures
		
		// clena up (this should be final method)
		Cleanup();
	}

}