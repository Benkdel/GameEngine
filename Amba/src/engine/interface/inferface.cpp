#include "inferface.h"


static bool isEntitySelected = false;
static EntityId selectedEntity;
static std::string entityName;


namespace Amba {

    const char* glsl_version = "#version 130";

    Interface::Interface()
    : m_Clear_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), m_MousePicker(MousePicker())
    {
    }

    void Interface::Setup(Window* window)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui_ImplGlfw_InitForOpenGL(window->getWindow(), true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // set up placeholders
        m_Clear_color = glm::vec4(0.45f, 0.55f, 0.60f, 1.00f);
        
        // Set up mouse Picker
        m_MousePicker.SetUp(window->GetWidth(), window->GetHeight());
    }

    void Interface::BindScene(Scene* scene)
    {
        // check if scene is null, where?
        p_CurrentScene = scene;
    }

    void Interface::Run(Camera &camera)
    {
        /*
            Positions and sizes of windows
            are being set in imgui.ini file for now (I'm not using docking just yet)
        */

        // update mouse picker
        m_MousePicker.UpdateMousePos(camera);


        // runs ImGui interface
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // main menu - right column
        {
            ImGui::Begin("Main Menu");
            ImGui::Text("Background Settings");
            ImGui::ColorEdit3("clear color", (float*)&m_Clear_color);

            ImGui::Spacing();
            ImGui::Text("FSP data");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::NewLine();
            ImGui::Text("Model Settings");
            ImGui::NewLine();

            // List of models to select
            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Avaiable Entities:");
            ImGui::Text("Active Entity: ");
            ImGui::SameLine();
            ImGui::Text(std::to_string(selectedEntity).c_str());

            ImGui::BeginListBox("##");
            {
                for (EntityId ent : SceneView<MeshComponent, TransformComponent>(p_CurrentScene))
                {
                    const bool isSelected = (selectedEntity == ent);
                    if (ImGui::Selectable(std::to_string(ent).c_str(), isSelected))
                    {
                        isEntitySelected = true;
                        selectedEntity = ent;
                    }
                }
            }
            ImGui::EndListBox();
            if (isEntitySelected)
            {
                ImGui::SliderFloat("Model - Size:", 
                        &p_CurrentScene->GetComponent<TransformComponent>(selectedEntity)->m_Size, 
                        0.001f, 100.0f, "%.03f");

                ImGui::SliderFloat3("Model - Position:", 
                        &p_CurrentScene->GetComponent<TransformComponent>(selectedEntity)->m_Position[0],
                        -50.0f, 50.0f, "%.03f");

                if (ImGui::Button("Duplicate"))
                {
                    p_CurrentScene->CopyEntity(selectedEntity);
                }

                // temporal
                glm::vec3& pos = p_CurrentScene->GetComponent<TransformComponent>(selectedEntity)->m_Position;
                int cellSize = p_CurrentScene->m_Spatial2DGrid.m_CellSize;
                int cell_x = pos.x / cellSize;
                int cell_z = pos.z / cellSize;
                
                ImGui::Text(std::to_string(cell_z * cellSize + cell_x).c_str());
            }

            ImGui::NewLine();
            ImGui::Text("Actions");
            
            if (ImGui::Button("Fly!"))
            {
                for (EntityId ent : SceneView<MeshComponent, TransformComponent>(p_CurrentScene))
                {
                    float x = (rand() % 10 + 1) - 5;
                    float y = (rand() % 10 + 1) - 5;
                    float z = (rand() % 10 + 1) - 5;
                    p_CurrentScene->GetComponent<TransformComponent>(ent)->m_Velocity = glm::vec3(x, y, z);
                }
            }

            if (ImGui::Button("Accelerate"))
            {
                glm::vec3 topSpeed = glm::vec3(25.0f);
                for (EntityId ent : SceneView<MeshComponent, TransformComponent>(p_CurrentScene))
                {
                    glm::vec3& velocity = p_CurrentScene->GetComponent<TransformComponent>(ent)->m_Velocity;
                    velocity += glm::vec3(1.0f);
                    
                    if (velocity.x > topSpeed.x)
                        velocity.x = topSpeed.x;
                    if (velocity.y > topSpeed.y)
                        velocity.y = topSpeed.y;
                    if (velocity.z > topSpeed.z)
                        velocity.z = topSpeed.z;
                }
            }

            if (ImGui::Button("Slow down"))
            {
                glm::vec3 minSpeed = glm::vec3(0.0f);
                for (EntityId ent : SceneView<MeshComponent, TransformComponent>(p_CurrentScene))
                {
                    glm::vec3& velocity = p_CurrentScene->GetComponent<TransformComponent>(ent)->m_Velocity;
                    velocity -= glm::vec3(1.0f);

                    if (velocity.x > minSpeed.x)
                        velocity.x = minSpeed.x;
                    if (velocity.y > minSpeed.y)
                        velocity.y = minSpeed.y;
                    if (velocity.z > minSpeed.z)
                        velocity.z = minSpeed.z;
                }
            }

            ImGui::NewLine();

            ImGui::End();
        }

        // Help menu
        {
            ImGui::Begin("Help Menu");

            ImGui::Text("Help Section");
            ImGui::Spacing();
            ImGui::Text("Press F10 to lock/unlock cursor");
            ImGui::Text("Press Spacebar to go up / ctrl to go down");
            ImGui::Text("Use AWSD keys to float arround");
            ImGui::Text("Close Engine using Escape");
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Interface::Cleanup()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
