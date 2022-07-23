#include "Model.h"

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/postprocess.h>

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
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |  aiProcess_FlipUVs | aiProcess_GenSmoothNormals);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            AB_ASSERT(false, "ERROR::ASSIMP::{0}", importer.GetErrorString());
        }
       
        m_Directory = path.substr(0, path.find_last_of('/') + 1);

        processNode(scene->mRootNode, scene);
    }

    void Model::LoadModelImporter(const std::string& path)
    {
        ABImp::Importer *importer;
        importer->LoadData("src/game/res/models/envirorment/tree_model_1/scene.gltf");

        m_Directory = path.substr(0, path.find_last_of('/') + 1);

        //processMeshImporter(importer);
    }

    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            
            vertex.v_Position = glm::vec3(
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            );

            vertex.v_Normals = glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );

            if (mesh->mTextureCoords[0])
            {
                vertex.v_TexCoords = glm::vec2(
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                );
            }
            else
            {
                vertex.v_TexCoords = glm::vec2(0.0f);
            }

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = LoadTextures(material, aiTextureType_DIFFUSE);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = LoadTextures(material, aiTextureType_SPECULAR);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> Model::LoadTextures(aiMaterial* mat, aiTextureType type)
    {
        std::vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++)
            {
                if (std::strcmp(m_TexturesLoaded[j].m_FilePath.data(), str.C_Str()) == 0)
                {
                    textures.push_back(m_TexturesLoaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture(m_Directory, str.C_Str(), type);
                texture.LoadTexture(false);
                textures.push_back(texture);
                m_TexturesLoaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }

    /*Mesh Model::processMeshImporter(ABImp::Importer* data)
    {
        unsigned int nMeshes = data->m_Meshes.size();

        for (unsigned int i = 0; i < nMeshes; i++)
        {
            // transform raw bin data to opengl format (this should be done in Importer)




        }
    }*/


}
