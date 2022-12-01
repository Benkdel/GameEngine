#include "Widgets.h"

namespace Amba {

    void ShowActiveEntitiesInScene(Scene* scene)
    {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Avaiable Entities:");
        ImGui::Text("Active Entity: ");
        ImGui::SameLine();
        ImGui::Text(Interface::GetActiveEntity(Interface::s_SelectedEntity).c_str());

        ImGui::BeginListBox("##");
        {
            for (EntityId ent : SceneView<MeshComponent, TransformComponent>(scene))
            {
                const bool isSelected = (Interface::s_SelectedEntity == ent);
                if (ImGui::Selectable(std::to_string(ent).c_str(), isSelected))
                {
                    Interface::s_SelectedEntity = ent;
                }
            }
        }
        ImGui::EndListBox();

        if (IsEntityValid(Interface::s_SelectedEntity))
        {
            TransformComponent& tsr = *scene->GetComponent<TransformComponent>(Interface::s_SelectedEntity);
            glm::vec3 position = tsr.GetPosition();
            ImGui::InputFloat3("EntPosition", &position[0], "% .03f");
            tsr.UpdatePosition(position);
        }
    }

}
