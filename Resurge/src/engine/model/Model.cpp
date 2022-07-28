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

        processNode(&importer->m_Nodes[0], importer);
     }

    void Model::processNode(ABImp::Node* node, ABImp::Importer* data)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < data->m_Nodes.size(); i++)
        {
            ABImp::Node* node = &data->m_Nodes[i];
            for (unsigned j = 0; j < node->meshes.size(); j++)
            {
                ABImp::Mesh* mesh = &data->m_Meshes[node->meshes[j]];
                m_Meshes.push_back(processMesh(mesh, node));
            }
        }
    }

    Mesh Model::processMesh(ABImp::Mesh* mesh, ABImp::Node *node)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        
        size_t nVertex = mesh->vertices.size();

        for (unsigned int j = 0; j < nVertex; j++)
        {
            Vertex vertex;

            vertex.v_Position.x = mesh->vertices[j].v_Position.x;
            vertex.v_Position.y = mesh->vertices[j].v_Position.y;
            vertex.v_Position.z = mesh->vertices[j].v_Position.z;

            vertex.v_Normals.x = mesh->vertices[j].v_Normals.x;
            vertex.v_Normals.x = mesh->vertices[j].v_Normals.y;
            vertex.v_Normals.z = mesh->vertices[j].v_Normals.z;

            vertex.v_TexCoords.x = mesh->vertices[j].v_TexCoords.x;
            vertex.v_TexCoords.y = mesh->vertices[j].v_TexCoords.y;
                
            vertices.push_back(vertex);
        }

        
        indices = mesh->indices;

        return Mesh(vertices, indices, node->tsrMatrix);
    }
}
