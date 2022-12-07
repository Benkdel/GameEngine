#include "Widgets.h"

#include <engine/dataTypes.h>

static glm::vec3 s_Position       = glm::vec3(0.0f);
static glm::vec3 s_Rotation       = glm::vec3(0.0f);
static glm::vec3 s_Scale          = glm::vec3(0.0f);

static glm::vec3 s_CamPosition    = glm::vec3(0.0f);
static glm::vec3 s_CamRotation    = glm::vec3(0.0f);
static glm::vec3 s_CamScaling     = glm::vec3(0.0f);

static EntityId s_PrevActiveCam = -1;

namespace Amba {

    // layout methods
    void ShowDiagnosticInfo(Scene* scene, glm::vec4 &clearColor)
    {
        ImGui::Text("Background Settings");
        ImGui::ColorEdit3("clear color", (float*)&clearColor);

        ImGui::Spacing();
        ImGui::Text("FSP data");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }

    void ShowActiveEntitiesInScene(Scene* scene)
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Avaiable Entities:");
        ImGui::Text("Active Entity: ");
        ImGui::SameLine();
        ImGui::Text(Interface::GetActiveEntityTag(Interface::s_SelectedEntity, scene).c_str());

        // create an entity wraper for editors camera
        if (ImGui::CollapsingHeader("Active Scene"))
        {
            if (ImGui::TreeNode("Scene Hierarchy"))
            {
                EntityId selected = -1;
                // TODO, should I implement an EditorComponent instead of using mesh to only get those into scene h?
                for (EntityId ent : SceneView<MeshComponent>(scene))
                {
                    std::string tag = scene->GetTag(ent);
                    if (ImGui::Selectable(tag.c_str(), selected == ent))
                        Interface::s_SelectedEntity = ent;
                }
                ImGui::TreePop();
            } 
        }
    }

    void ShowTransform(Scene* scene)
    {
        if (IsEntityValid(Interface::s_SelectedEntity))
        {
            TransformComponent* tsr = scene->GetComponent<TransformComponent>(Interface::s_SelectedEntity);
            if (tsr == nullptr)
                return;

            int componentId = GetColliderTypeIndex(Interface::s_SelectedEntity, scene);
            ColliderComponent* collider = scene->GetComponentWithId<ColliderComponent>(Interface::s_SelectedEntity, componentId);

            if (ImGui::CollapsingHeader("Transform"))
            {
                if (ImGui::TreeNode("Transform"))
                {
                    // position
                    s_Position = tsr->GetPosition();
                    TransformWidget("Position: ", s_Position);
                    tsr->UpdatePosition(s_Position);
                    // rotation
                    s_Rotation = tsr->GetRotationAxis();
                    TransformWidget("Rotation: ", s_Rotation);
                    tsr->UpdateRotationAxis(s_Rotation);
                    // scaling
                    s_Scale = tsr->GetScale();
                    TransformWidget("Scaling: ", s_Scale);
                    tsr->UpdateScale(s_Scale);

                    if (collider != nullptr)
                    {
                        collider->TransformCollider(tsr);
                    }

                    ImGui::TreePop();
                }
            }
        }
    }

    void ShowAdditionalEntitiesData(Scene* scene)
    {
        if (!IsEntityValid(Interface::s_SelectedEntity))
            return;

        ImGui::NewLine();
        ImGui::Text("Selected Entity Cell: ");
        ImGui::SameLine();

        TransformComponent* tsr = scene->GetComponent<TransformComponent>(Interface::s_SelectedEntity);
        if (tsr == nullptr)
            return;

        GridCell gridCell = scene->m_Spatial2DGrid->GetGridCell(tsr->GetPosition());
        
        int cellIdx = -1;

        if (gridCell.IsCellValid())
        {
            Cell& cell = gridCell.GetCell();
            cellIdx = cell.GetCellIdx();
        }

        ImGui::Text(std::to_string(cellIdx).c_str());
    }

    void ShowEntityUnderMouse(Scene* scene)
    {
        if (IsEntityValid(Interface::s_EntUnderCursor))
        {
            ImGui::NewLine();
            ImGui::Text("Entity being hovered: ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(Interface::s_EntUnderCursor).c_str());
        }
    }

    void ShowCameraSelection(Scene* scene, Camera* activeCamera, bool& editorsCameraActive)
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Avaiable Cameras:");
        ImGui::Text("Active Camera: ");
        ImGui::SameLine();
        ImGui::Text((Interface::GetActiveEntityTag(Interface::s_SelectedCameraEntity, scene)).c_str());

        if (!IsEntityValid(Interface::s_SelectedCameraEntity))
        {
            Interface::s_SelectedCameraEntity = scene->GetEntity(EDITOR_CAMERA_TAG);
            s_PrevActiveCam = Interface::s_SelectedCameraEntity;
        }

        if (ImGui::CollapsingHeader("Camera Selection"))
        {
            if (ImGui::TreeNode("Cameras"))
            {
                EntityId selected = -1;
                for (EntityId ent : SceneView<CameraComponent>(scene))
                {
                    std::string tag = scene->GetTag(ent);
                    if (ImGui::Selectable(tag.c_str(), selected == ent))
                        Interface::s_SelectedCameraEntity = ent;
                }
                ImGui::TreePop();
            }
        }

        // set new camera as primary
        if (IsEntityValid(Interface::s_SelectedCameraEntity))
        {
            // update selected camera
            if (Interface::s_SelectedCameraEntity != s_PrevActiveCam)
            {
                activeCamera = scene->GetComponent<CameraComponent>(Interface::s_SelectedCameraEntity)->GetCamera();
                scene->SetActiveCamera(Interface::s_SelectedCameraEntity);

                if (Interface::GetActiveEntityTag(Interface::s_SelectedCameraEntity, scene) == EDITOR_CAMERA_TAG)
                    editorsCameraActive = true;
                else
                    editorsCameraActive = false;
            
                s_PrevActiveCam = Interface::s_SelectedCameraEntity;
            }

            TransformComponent* tsr = scene->GetComponent<TransformComponent>(Interface::s_SelectedCameraEntity);
            if (ImGui::TreeNode("Camera Transform"))
            {
                // position
                s_CamPosition = tsr->GetPosition();
                TransformWidget("CamPosition: ", s_CamPosition);
                tsr->UpdatePosition(s_CamPosition);
                // rotation
                s_CamRotation = tsr->GetRotationAxis();
                TransformWidget("CamRotation: ", s_CamRotation);
                tsr->UpdateRotationAxis(s_Rotation);
                // scaling
                s_CamScaling = tsr->GetScale();
                TransformWidget("CamScaling: ", s_CamScaling);
                tsr->UpdateScale(s_CamScaling);

                ImGui::TreePop();
            }
        }        
    }

}


// Basic Widget methods
namespace Amba {

    void TransformWidget(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.4f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.3f, 1.0f });
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f);
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }

}

// debug tools

namespace Amba {

    static bool GenMethodActive = true;

    // frame for all debug methods - general, temporal info I need while building

    void DebugingMethods(Scene* scene)
    {
       if (!GenMethodActive)
           return;

       if (ImGui::CollapsingHeader("DebugData"))
       {
           if (ImGui::TreeNode("Collider Data: "))
           {
               ColliderData(scene);
               ImGui::TreePop();
           }

           // add more as needed

       }
    }





    // debug methods

    void ColliderData(Scene* scene)
    {

        if (!IsEntityValid(Interface::s_SelectedEntity))
            return;

        ImGui::NewLine();
        ImGui::Text("Collider data");

        int componentId = GetColliderTypeIndex(Interface::s_SelectedEntity, scene);
        ColliderComponent* collider = scene->GetComponentWithId<ColliderComponent>(Interface::s_SelectedEntity, componentId);

        if (collider == nullptr)
            return;

        if (collider->GetType() == ColliderComponent::TYPE_SPHERE)
        {
            SphereCollider* sphere = dynamic_cast<SphereCollider*>(collider);
            if (sphere == NULL)
                return;

            ImGui::Text("Position: ");
            ImGui::SameLine();
            ImGui::Text(VecToStr(sphere->GetCenter()).c_str());
            ImGui::Text("Radius: ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(sphere->GetRadius()).c_str());

        }
        else if (collider->GetType() == ColliderComponent::TYPE_AAB)
        {
            AABCollider* aab = dynamic_cast<AABCollider*>(collider);
            if (aab == NULL)
                return;

            ImGui::Text("Transforming colliders has no efect for now");
            glm::vec3 center = aab->GetCenter();
            TransformWidget("Collider Center", center);

            glm::vec3 MaxExtend = aab->GetMaxExtents();
            TransformWidget("Collider MaxExtend", MaxExtend);

            glm::vec3 MinExtend = aab->GetMinExtents();
            TransformWidget("Collider MinExtend", MinExtend);

        }
        else
        {
            PlaneCollider* plane = dynamic_cast<PlaneCollider*>(collider);
            if (plane == NULL)
                return;

            ImGui::Text("Transforming colliders has no efect for now");
            glm::vec3 center = plane->GetCenter();
            TransformWidget("Collider Center", center);

            glm::vec3 MaxExtend = plane->GetMaxExtents();
            TransformWidget("Collider MaxExtend", MaxExtend);

            glm::vec3 MinExtend = plane->GetMinExtents();
            TransformWidget("Collider MinExtend", MinExtend);
        }

    }
}
