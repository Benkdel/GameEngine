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
		return 0;
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

	struct vec3 {
		float x;
		float y;
		float z;
	};

	struct vec2 {
		float x;
		float y;
	};

	struct Vertex {
		vec3 v_Position;
		vec3 v_Normals;
		vec2 v_TexCoords;
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

		Importer();

		void LoadData(const std::string& filePath);						// Loads data from file

		std::vector<impMeshIndexes> m_Meshes;							// structs with indexes or relationships
		std::vector<impAccesor> m_Accessors;							// structs with indexes or relationships
		std::vector<impBufferView> m_BufferViews;						// structs with indexes or relationships
		std::vector<impBuffer> m_Buffers;								// structs with indexes or relationships
						
		std::vector<Vertex> m_Vertices;									// OPENGL format data
		std::vector<unsigned int> m_Indices;							// OPENGL format data

		void Cleanup();													// Clean all date

	private:
		std::vector<std::vector<unsigned char>> m_BinData;				// vector or vectors of bytes holding raw data from binary files

		std::vector<float> m_Positions;									// vector to group floats before convert to vertices
		std::vector<float> m_Normals;									// vector to group floats before convert to vertices 
		std::vector<float> m_Texcoords;									// vector to group floats before convert to vertices

		std::string m_Directory;										// directory of all data needed

		/*
		=======================
		LOAD STRUCTURES
		=======================
		*/

		void GetMeshes();
		void GetAccessors();
		void GetBufferViews();
		void GetBuffers();
	
		/*
		=================================
		Translate data into OPENGL format
		=================================
		*/
		void GetBinData();
		void GetMeshData();
		// todo
		void GetMaterials();

		// translate data to vertices
		void GenVertices();

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

