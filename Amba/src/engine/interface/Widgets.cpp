#include "Widgets.h"

static glm::vec3 position = glm::vec3(0.0f);
static glm::vec3 rotation = glm::vec3(0.0f);
static glm::vec3 scale = glm::vec3(0.0f);

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
        ImGui::Text(Interface::GetActiveEntity(Interface::s_SelectedEntity).c_str());

        if (ImGui::CollapsingHeader("Active Scene"))
        {
            if (ImGui::TreeNode("Scene Hierarchy"))
            {
                EntityId selected = -1;
                for (EntityId ent : SceneView<>(scene))
                {
                    std::string name = "Ent: " + std::to_string(ent);
                    if (ImGui::Selectable(name.c_str(), selected == ent))
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
                    position = tsr->GetPosition();
                    TransformWidget("Position: ", position);
                    tsr->UpdatePosition(position);
                    // rotation
                    rotation = tsr->GetRotationAxis();
                    TransformWidget("Rotation: ", rotation);
                    tsr->UpdateRotationAxis(rotation);
                    // scaling
                    scale = tsr->GetScale();
                    TransformWidget("Scaling: ", scale);
                    tsr->UpdateScale(scale);

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
