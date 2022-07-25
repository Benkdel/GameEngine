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
		GetMeshes();
		GetAccessors();
		GetBufferViews();
		GetBuffers();

		/* PROCESS DATA INTO OPENGL FORMAT */
		GetBinData();
		GetMeshData();
		GetMaterials();
		GenVertices();
	}

	void Importer::Cleanup()													// Clean all data
	{
		m_BinData.clear();
		m_Positions.clear();
		m_Normals.clear();
		m_Texcoords.clear();
		m_Data.clear();
		m_Meshes.clear();
		m_Accessors.clear();
		m_BufferViews.clear();
		m_Buffers.clear();
		m_Vertices.clear();
		m_Indices.clear();
		m_Directory = "";
	}

	void Importer::GetMeshes()
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
			m_Meshes.push_back({ att, indices, material, mode });
		}
	}

	void Importer::GetAccessors()
	{
		for (auto& accesor : m_Data["accessors"])
		{
			unsigned int bufferView = accesor["bufferView"];
			unsigned int byteOffset = accesor.value("byteOffset", 0);
			unsigned int componentType = accesor["componentType"];
			unsigned int count = accesor["count"];
			std::string type = accesor["type"];
			unsigned int size = GetNumOfElements(type) * GetSizeOfComponent(componentType);

			m_Accessors.push_back({ bufferView, byteOffset, componentType, count, type, size });
		}
	}

	void Importer::GetBufferViews()
	{
		for (auto& bufferView : m_Data["bufferViews"])
		{
			unsigned int buffer = bufferView["buffer"];
			unsigned int byteLength = bufferView["byteLength"];
			unsigned int byteOffset = bufferView.value("byteOffset", 0);
			unsigned int byteStride = bufferView.value("byteStride", 0);
			std::string name = bufferView.value("name", "null");
			unsigned int target = bufferView.value("target", -1);

			m_BufferViews.push_back({ buffer, byteLength, byteOffset, byteStride, name, target });
		}
	}

	void Importer::GetBuffers()
	{
		for (auto& buffer : m_Data["buffers"])
		{
			unsigned int byteLength = buffer["byteLength"];
			std::string uri = buffer["uri"];

			m_Buffers.push_back({ byteLength, uri });
		}
	}

	/*
	=================================
	Translate data into OPENGL format
	=================================
	*/

	void Importer::GetBinData()
	{
		for (auto& buffer : m_Buffers)
		{
			std::string path = m_Directory + buffer.uri;
			std::ifstream input(path);

			AB_ASSERT(input.is_open(), ("Failed to open bin file at {0}", path));

			std::stringstream ss;
			ss << input.rdbuf();
			std::string bytesText = ss.str();

			std::vector<unsigned char> bytes(bytesText.begin(), bytesText.end());
			m_BinData.push_back(bytes);
		}
	}

	void Importer::GetMeshData()
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
		{
			/* loop through meshes */
			impMeshIndexes mesh = m_Meshes.at(i);

			/* every mesh has primitives used by accesors */
			if (mesh.indices != -1)
			{
				impAccesor accesor = m_Accessors.at(mesh.indices);
				impBufferView bufferView = m_BufferViews.at(accesor.bufferView);
				std::vector<unsigned char> data = m_BinData.at(bufferView.buffer);
				const unsigned int beginningOfData = bufferView.byteOffset + accesor.byteOffset;
				const unsigned int lenghtOfData = accesor.count * GetSizeOfComponent(accesor.componentType) * GetNumOfElements(accesor.type);

				for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i += bufferView.byteStride)
				{
					unsigned int value = 0;
					ConvertBytes<unsigned int>(value, data, accesor, i, beginningOfData + lenghtOfData);
					m_Indices.push_back((unsigned int)value);
				}
			}

			if (mesh.attributes.Positions != -1)
			{
				impAccesor accesor = m_Accessors.at(mesh.attributes.Positions);
				impBufferView bufferView = m_BufferViews.at(accesor.bufferView);
				std::vector<unsigned char> data = m_BinData.at(bufferView.buffer);
				const unsigned int beginningOfData = bufferView.byteOffset + accesor.byteOffset;
				const unsigned int lenghtOfData = accesor.count * GetSizeOfComponent(accesor.componentType) * GetNumOfElements(accesor.type);

				for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i += bufferView.byteStride)
				{
					float value = 0.0f;
					ConvertBytes<float>(value, data, accesor, i, beginningOfData + lenghtOfData);
					m_Positions.push_back((float)value);
				}
			}

			if (mesh.attributes.Normals != -1)
			{
				impAccesor accesor = m_Accessors.at(mesh.attributes.Normals);
				impBufferView bufferView = m_BufferViews.at(accesor.bufferView);
				std::vector<unsigned char> data = m_BinData.at(bufferView.buffer);
				const unsigned int beginningOfData = bufferView.byteOffset + accesor.byteOffset;
				const unsigned int lenghtOfData = accesor.count * GetSizeOfComponent(accesor.componentType) * GetNumOfElements(accesor.type);

				for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i)
				{
					float value = 0.0f;
					ConvertBytes<float>(value, data, accesor, i, beginningOfData + lenghtOfData);
					m_Normals.push_back((float)value);
				}
			}

			if (mesh.attributes.Texcoords != -1)
			{
				impAccesor accesor = m_Accessors.at(mesh.attributes.Texcoords);
				impBufferView bufferView = m_BufferViews.at(accesor.bufferView);
				std::vector<unsigned char> data = m_BinData.at(bufferView.buffer);
				const unsigned int beginningOfData = bufferView.byteOffset + accesor.byteOffset;
				const unsigned int lenghtOfData = accesor.count * GetSizeOfComponent(accesor.componentType) * GetNumOfElements(accesor.type);

				for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i)
				{
					float value = 0.0f;
					ConvertBytes<float>(value, data, accesor, i, beginningOfData + lenghtOfData);
					m_Texcoords.push_back((float)value);
				}
			}
		}
	}

	// todo
	void Importer::GetMaterials()
	{
	}

	// translate data to vertices
	void Importer::GenVertices()
	{
		// all vectors corresponding to vertices should be same size
		// divided by num elementss
		size_t elements = m_Positions.size() / 3; // pos is vec3 - every 3 floats ==> 1 vec3
		unsigned int posIdx = 0;
		unsigned int normIdx = 0;
		unsigned int texIdx = 0;

		for (unsigned int i = 0; i < elements; i++)
		{
			Vertex vertex;

			if (m_Positions.size() > 0)
			{
				vertex.v_Position.x = m_Positions[posIdx];
				vertex.v_Position.y = m_Positions[posIdx + 1];
				vertex.v_Position.z = m_Positions[posIdx + 2];
				posIdx += 3;
			}

			if (m_Normals.size() > 0)
			{
				vertex.v_Normals.x = m_Normals[normIdx++];
				vertex.v_Normals.y = m_Normals[normIdx++];
				vertex.v_Normals.z = m_Normals[normIdx++];
			}

			if (m_Texcoords.size() > 0)
			{
				vertex.v_TexCoords.x = m_Texcoords[texIdx++];
				vertex.v_TexCoords.y = m_Texcoords[texIdx++];
			}
			m_Vertices.push_back(vertex);
		}
	}

}