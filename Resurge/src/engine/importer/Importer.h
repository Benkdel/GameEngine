#pragma once

// my OWN header only GLTF importer
// Because ASSIMP was to heavy and hard to debug
// And there were not many tutorials online

/*
TODO LIST

	0. Importer now has basic vectors with data needed for draw: buffer, buffer views, accesors, mesh
	1. try to render tree model (has 1 node, 1 mesh)
	2. add recursive getter for nodes and meshes and build parent child relationship (linked list of data??)
	3. add MATERIALS, IMAGES, TEXTURE SAMPLERS, MIG MAG
	4. add matrix transformations
	5. add animations

	6. load models using multithread (maby after I have all structs with data, load from binary file using multi thread)
+


*/

#include <NLOHMANN/json.hpp>

#include <engine/core.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>


using json = nlohmann::json;

static unsigned int GetNumOfElements(const std::string& type)
{
	if (type == "MAT4") return 4;
	if (type == "MAT3") return 3;
	if (type == "VEC3") return 3;
	if (type == "VEC2") return 2;
	if (type == "SCALAR") return 1;
	AB_ASSERT(false, "Type not found");
	return 0;
}

static unsigned int GetSizeOfComponent(const unsigned int& componentType)
{
	switch (componentType)
	{
	case 5120: return sizeof(char);
	case 5121: return sizeof(unsigned char);
	case 5122: return sizeof(short);
	case 5123: return sizeof(unsigned short);
	case 5124: return sizeof(int);
	case 5125: return sizeof(unsigned int);
	case 5126: return sizeof(float);
	case 5127: return sizeof(double);
	default:
		AB_ASSERT(false, "Component Type not found");
		break;
	}
}

namespace ABImp {														// documentation

	template<typename T>
	class Array {														// custome array to build with dynamic size at run time
	public:
		Array(unsigned int size)
			: m_Data(0), m_Size(size)
		{
			m_Data = (T*)malloc(sizeof(T) * m_Size);
			AB_ASSERT(m_Data != NULL, "Failed to allocate memory");

			m_CurrentIdx = 0;
		}
		
		~Array()
		{
			for (unsigned int i = 0; i < m_Size; i++)
				m_Data[i] = NULL;
			m_Data = NULL;
		}

		T& operator[](T index)
		{
			AB_ASSERT(index < m_CurrentIdx, "Invalid memory reading");
			return m_Data[index];
		}


		// other methods like sorting, deleting, etc
		void PushBack(T value)
		{
			m_Data[m_CurrentIdx] = value;
			m_CurrentIdx++;
		}

		T* GetData()
		{
			return m_Data;
		}
	
	private:
		T* m_Data;
		unsigned int m_Size;
		unsigned int m_CurrentIdx;
	};


	struct Vertex {
		glm::vec3 v_Position;
		glm::vec3 v_Normals;
		glm::vec2 v_TexCoords;
	};

	struct impNodes {													// To be done, i dont fully understand it yet

	};

	struct impAttributesIndexes {										// Mesh - stores indexes of attributes - pointing to accesors array
		unsigned int Positions;											//		index to be used in ACCESSORS (identifieds accesor at idx [] to be of POSITIONS)
		unsigned int Normals;											//		index to be used in ACCESORS 
		unsigned int Texcoords;											//		index to be used in ACCESORS
	};

	struct impMeshIndexes {												// Mesh - stores indexes for mesh - pointing to accesors array
		impAttributesIndexes attributes;								//		stores attribute indexes pointing to accesors array
		unsigned int indices;											//		index to be used in ACCESSORS
		unsigned int material;											//		index to be used in ACCESSORS
		unsigned int mode;												//		index to be used in ACCESSORS - IM NOT SURE ABOUT THIS ONE YET
	};

	struct impAccesor {													// stores accesors data - pointing to buffer views
		unsigned int bufferView;										//		index of buffer view that this accesor explains
		unsigned int byteOffset;										//		similar to STRIDE parameters in typical layout (review)
		unsigned int componentType;										//		char, unsigned char, float, double, etc
		unsigned int count;												//		number of items: ie, 3 VEC2, 3 SCALARS, 2 VEC3, etc
		std::string type;												//		type: VEC3, VEC2, SCALAR
		unsigned int chunkSize;											//		computed by importer: size of group of data: ie --> vec3, vec2, int, etc (vec3 would be 3 times 4 bytes if float, etc...)
	};

	struct impBufferView {												//	describes the binary data from URI of a given buffer
		unsigned int buffer;											//		buffer idx that view its describing
		unsigned int byteLength;										//		lenght in bytes of chung of data that view is describing
		unsigned int byteOffset;										//		where the chunk of data begins
		unsigned int byteStride;										//		review this one, im not sure about this byte stride
		std::string name;												//		name used to identify buffer
		unsigned int target;											//		id of openGL buffer such as ARRAY_BUFFER, for example
	};

	struct impBuffer {													// contains the raw binary file with data
		unsigned int byteLength;										//		length of bin file in bytes
		std::string uri;												//		path to bin file
	};

	class Importer {
	public:

		json m_Data;

		void LoadData(const std::string& filePath)
		{
			std::ifstream input(filePath);
			m_Data = json::parse(input);
			input.close();

			m_Directory = filePath.substr(0, filePath.find_last_of('/') + 1);

			std::string uri = m_Data["buffers"][0]["uri"];				// for now im assuming that only 1 buffer exists, just for testing purposes
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

		std::vector<impMeshIndexes> m_Meshes;											// structs with indexes or relationships
		std::vector<impAccesor> m_Accessors;											// structs with indexes or relationships
		std::vector<impBufferView> m_BufferViews;										// structs with indexes or relationships
		std::vector<impBuffer> m_Buffers;												// structs with indexes or relationships
						
		std::vector<Vertex> m_Vertices;													// OPENGL format data
		std::vector<unsigned int> m_Indices;											// OPENGL format data

	private:
		std::vector<std::vector<unsigned char>> m_BinData;								// vector or vectors of bytes holding raw data from binary files

		std::vector<float> m_Positions;													// vector to group floats before convert to vertices
		std::vector<float> m_Normals;													// vector to group floats before convert to vertices 
		std::vector<float> m_Texcoords;													// vector to group floats before convert to vertices

		std::string m_Directory;

		/*
		=======================
		LOAD STRUCTURES
		=======================
		*/

		void GetMeshes()
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

		void GetAccessors()
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

		void GetBufferViews()
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

		void GetBuffers()
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

		void GetBinData()
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

		void GetMeshData()
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
					
					for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i)
					{
						unsigned int value;
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

					for (unsigned int i = beginningOfData; i < beginningOfData + lenghtOfData; i)
					{
						float value;
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
						float value;
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
						float value;
						ConvertBytes<float>(value, data, accesor, i, beginningOfData + lenghtOfData);
						m_Texcoords.push_back((float)value);
					}
				}
			}
		}

		// todo
		void GetMaterials()
		{

		}

		// translate data to vertices
		void GenVertices()
		{
			// all vectors corresponding to vertices should be same size
			// divided by num elementss
			unsigned int elements = m_Positions.size() / 3; // i know every 3 points is a pos, and positions must always be there
			unsigned int posIdx = 0;
			unsigned int normIdx = 0;
			unsigned int texIdx = 0;

			for (unsigned int i = 0; i < elements; i++)
			{
				Vertex vertex;

				if (m_Positions.size() > 0)
				{
					vertex.v_Position = glm::vec3(
						m_Positions[posIdx++],
						m_Positions[posIdx++],
						m_Positions[posIdx++]
					);				
				}

				if (m_Normals.size() > 0)
				{
					vertex.v_Normals = glm::vec3(
						m_Normals[normIdx++],
						m_Normals[normIdx++],
						m_Normals[normIdx++]
					);
				}

				if (m_Texcoords.size() > 0)
				{
					vertex.v_TexCoords = glm::vec2(
						m_Normals[texIdx++],
						m_Normals[texIdx++]
					);
				}
				m_Vertices.push_back(vertex);
			}
		}

		/*
		=========================
		HELPER FUNCTIONS
		=========================
		*/

		template<typename T>
		void ConvertBytes(T& value, std::vector<unsigned char>& data, const impAccesor& accesor, unsigned int& idx, const unsigned int arraySize)
		{
			Array bytes = Array<unsigned char>(arraySize);

			for (unsigned int j = 0; j < GetSizeOfComponent(accesor.componentType); j++)
			{
				bytes.PushBack(data[idx++]);
			}
			std::memcpy(&value, bytes.GetData(), GetSizeOfComponent(accesor.componentType));
		}
	};
}

