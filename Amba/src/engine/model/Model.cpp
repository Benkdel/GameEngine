#include "Model.h"

#include <engine/core.h>
#include <engine/ecs/Entity.h>

#include <engine/ResourceManager.h>

namespace Amba {
    
    Model::Utils* Model::s_Utils = new Model::Utils;

    void Model::LoadModel(const std::string& path, Entity* parentEntity)
    {
        ABImp::Importer* importer = new ABImp::Importer();
        importer->LoadData(path);

        s_Utils->m_Directory = path.substr(0, path.find_last_of('/') + 1);

        ProcessNode(&importer->m_Nodes[0], importer, parentEntity);

        ResetUtils();
     }

    void Model::ProcessNode(ABImp::Node* node, ABImp::Importer* data, Entity* parentEntity)
    {
        // process all the node's meshes (if any)
        Entity* current = parentEntity;
        AB_ASSERT(!(current == NULL), "Entity is null!");

        unsigned int counter = 1;

        for (unsigned int i = 0; i < data->m_Nodes.size(); i++)
        {
            ABImp::Node* node = &data->m_Nodes[i];
            
            for (unsigned j = 0; j < node->meshes.size(); j++)
            {
                current->AddComponent<MeshComponent>();
                
                ABImp::Mesh* mesh = &data->m_Meshes[node->meshes[j]];
                
                ProcessMesh(mesh, node, current);

                if (current->p_Child == nullptr)
                {
                    // create child and advance current node to it
                    //current->p_Child = new Entity(current->GetScene(), "Child[" + std::to_string(counter++) + "]");
                   // AB_ASSERT(!(current->p_Child == NULL), "Error allocating memory!");
                   // current = current->p_Child;
                }
            }
        }
    }

    void Model::ProcessMesh(ABImp::Mesh* mesh, ABImp::Node *node, Entity* ent)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture*> textures;
        
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

        if (mesh->material.uri.size() > 0)
        {
            // for now I am only using 1 texture for each mesh
            std::vector<Texture*> temp;
            if (mesh->material.uri.find(ABImp::TexType::NORMAL) != mesh->material.uri.end())
            {
                textures.push_back(LoadTextures(mesh->material.uri[ABImp::TexType::NORMAL], "texture"));
            }

            if (mesh->material.uri.find(ABImp::TexType::PBR_METALLIC) != mesh->material.uri.end())
            {
                textures.push_back(LoadTextures(mesh->material.uri[ABImp::TexType::PBR_METALLIC], "texture"));
            }
            if (mesh->material.uri.find(ABImp::TexType::METALLIC) != mesh->material.uri.end())
            {
                textures.push_back(LoadTextures(mesh->material.uri[ABImp::TexType::METALLIC], "texture"));
            }
        
            ent->GetComponent<MeshComponent>()->m_Textures = textures;            
        }
        
        ent->GetComponent<MeshComponent>()->m_Vertices = vertices;
        ent->GetComponent<MeshComponent>()->m_Indices = indices;
        ent->GetComponent<TransformComponent>()->SetTransformMatrix(node->tsrMatrix);
    }

    void Model::ResetUtils()
    {
        s_Utils->m_Directory = "";
        s_Utils->m_TexturesLoaded.clear();
    }

    Texture* Model::LoadTextures(std::string uri, std::string texName)
    {
        // for now only one texture for every mesh

        Texture* texture;
        bool skip = false;

        for (unsigned int i = 0; i < s_Utils->m_TexturesLoaded.size(); i++)
        {
            if (std::strcmp(s_Utils->m_TexturesLoaded[i]->GetPath().data(), uri.c_str()) == 0)
            {
                texture = s_Utils->m_TexturesLoaded[i];
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            ResManager::GenTexture(s_Utils->m_Directory + uri, texName + uri);
            texture = ResManager::GetTexture(texName + uri);
            texture->LoadTexture(false);
            
            s_Utils->m_TexturesLoaded.push_back(texture);
        }
        return texture;
    }
}
