#include "Model.h"

#include <engine/core.h>

namespace Amba {
    
    Model::Model()
    {
    }

    Model::~Model()
    {
    }

    void Model::Cleanup()
    {
    }

    void Model::LoadModel(const std::string& path)
    {
        ABImp::Importer* importer = new ABImp::Importer();
        importer->LoadData(path);

        m_Directory = path.substr(0, path.find_last_of('/') + 1);

        processMesh(importer);
       
        //importer->Cleanup();
     }

    void Model::processMesh(ABImp::Importer* data)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        
        size_t nMeshes = data->m_Meshes.size();

        for (size_t i = 0; i < nMeshes; i++)
        {
            size_t nVertex = data->m_Vertices.size();

            for (unsigned int j = 0; j < nVertex; j++)
            {
                Vertex vertex;

                vertex.v_Position.x = data->m_Vertices[j].v_Position.x;
                vertex.v_Position.y = data->m_Vertices[j].v_Position.y;
                vertex.v_Position.z = data->m_Vertices[j].v_Position.z;

                vertex.v_Normals.x = data->m_Vertices[j].v_Normals.x;
                vertex.v_Normals.x = data->m_Vertices[j].v_Normals.y;
                vertex.v_Normals.z = data->m_Vertices[j].v_Normals.z;

                vertex.v_TexCoords.x = data->m_Vertices[j].v_TexCoords.x;
                vertex.v_TexCoords.y = data->m_Vertices[j].v_TexCoords.y;
                
                vertices.push_back(vertex);
            }
            indices = data->m_Indices;
            m_Meshes.push_back(Mesh(vertices, indices));
        }
    }
}
