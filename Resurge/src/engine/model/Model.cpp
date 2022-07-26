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

        processNode(importer);
     }

    void Model::processNode(ABImp::Importer* data)
    {
        for (auto& node : data->m_Nodes)
        {
            for (auto& meshIdx : node.meshesIdx)
            {
                processMesh(data->m_Meshes.at(meshIdx));
            }
        }
    }

    void Model::processMesh(ABImp::Mesh& mesh)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        
        size_t nVertex = mesh.vertices.size();

        for (unsigned int j = 0; j < nVertex; j++)
        {
            Vertex vertex;

            vertex.v_Position.x = mesh.vertices[j].v_Position.x;
            vertex.v_Position.y = mesh.vertices[j].v_Position.y;
            vertex.v_Position.z = mesh.vertices[j].v_Position.z;

            vertex.v_Normals.x = mesh.vertices[j].v_Normals.x;
            vertex.v_Normals.x = mesh.vertices[j].v_Normals.y;
            vertex.v_Normals.z = mesh.vertices[j].v_Normals.z;

            vertex.v_TexCoords.x = mesh.vertices[j].v_TexCoords.x;
            vertex.v_TexCoords.y = mesh.vertices[j].v_TexCoords.y;
                
            vertices.push_back(vertex);
        }

        TSR tsr;
        tsr.translation.x = mesh.translation.x;
        tsr.translation.y = mesh.translation.y;
        tsr.translation.z = mesh.translation.z;
        
        tsr.scaling.x = mesh.scaling.x;
        tsr.scaling.y = mesh.scaling.y;
        tsr.scaling.z = mesh.scaling.z;

        tsr.rotation.angle = mesh.rotation.angle;
        tsr.rotation.vec3.x = mesh.rotation.vec3.x;
        tsr.rotation.vec3.y = mesh.rotation.vec3.y;
        tsr.rotation.vec3.z = mesh.rotation.vec3.z;

        indices = mesh.indices;

        m_Meshes.push_back(Mesh(vertices, indices, tsr));
    }
}
