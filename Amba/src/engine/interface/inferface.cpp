#include "inferface.h"

#include <engine/interface/Widgets.h>

#include <engine/io/Keyboard.h>
#include <engine/io/Mouse.h>


namespace Amba {

    EntityId Interface::s_SelectedEntity = -1;
    EntityId Interface::s_EntUnderCursor = -1;

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
        m_MousePicker.UpdateScene(p_CurrentScene);
    }

    void Interface::Run(Camera &camera)
    {
        /*
            Positions and sizes of windows
            are being set in imgui.ini file for now (I'm not using docking just yet)
        */

        // update mouse picker
        m_MousePicker.UpdateMousePos(camera);

        m_Status = (Amba::Mouse::isMouseLocked() == true) ? STATUS::ACTIVE : STATUS::INNACTIVE;

        if (m_Status == STATUS::ACTIVE)
            s_EntUnderCursor = m_MousePicker.SelectEntity(camera);

        // user input
        if (m_Status == STATUS::ACTIVE)
        {
            // Select entity
            if (Amba::KeyBoard::KeyWentDown(GLFW_KEY_P))
                Interface::s_SelectedEntity = s_EntUnderCursor;

            // Apply force - temporal
            if (Amba::KeyBoard::KeyWentDown(GLFW_KEY_LEFT))
                p_CurrentScene->GetComponent<PhysicsComponent>(Interface::s_SelectedEntity)->IncreaseForce(glm::vec3(-0.5f, 0.0f, 0.0f));
            if (Amba::KeyBoard::KeyWentDown(GLFW_KEY_RIGHT))
                p_CurrentScene->GetComponent<PhysicsComponent>(Interface::s_SelectedEntity)->IncreaseForce(glm::vec3(0.5f, 0.0f, 0.0f));
        }
        
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
            
            ShowActiveEntitiesInScene(p_CurrentScene);

            ImGui::End();
        }


        //TestWidget();

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

    std::string Interface::GetActiveEntity(EntityId ent)
    {
        if (Amba::IsEntityValid(ent))
            return std::to_string(ent);
        return "-";
    }
}
