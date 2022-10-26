#include "inferface.h"


// Models 
static std::string currentModel = "No active model selected";
static bool selection = false;

namespace Amba {

    const char* glsl_version = "#version 130";

    Interface::Interface() 
    : m_Clear_color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), m_Window(nullptr)
    {}


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
        
    }

    void Interface::Run()
    {
        /*
            Positions and sizes of windows
            are being set in imgui.ini file for now (I'm not using docking just yet)
        */

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

            ImGui::TextColored(ImVec4(1, 1, 0, 1), "Avaiable models:");
            ImGui::Text("Active model: ");
            ImGui::SameLine();
            ImGui::Text(currentModel.c_str());

            // List of models to select
            ImGui::BeginListBox("##");
            for (auto k : ResManager::rm_Models)
            {
                // get current selection and set
                // it in current model, using string name
                const bool isSelected = (currentModel == k.first);
                if (ImGui::Selectable(k.first.c_str(), isSelected))
                {
                    selection = true;
                    currentModel = k.first;
                }
            }
            ImGui::EndListBox();

            if (selection)
            {
                ImGui::SliderFloat("Size:", &ResManager::rm_Models[currentModel]->m_Size, 0.001f, 1.0f, "%.003f");
                ImGui::SliderFloat3("Position:", &ResManager::rm_Models[currentModel]->m_Translation[0], -500.0f, 500.0f, "%.003f");
            }

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