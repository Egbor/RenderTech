#include "Engine/Core/Render/Gui/GuiWindow.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Engine {
    GuiWindow::GuiWindow(const String& tag) 
        : GuiLayout(tag) {

    }

    void GuiWindow::AddWidget(GuiWidget* widget, GuiSplitDirection direction, Float ratio) {
        m_nodes[widget->GetName()] = { direction, ratio };
        AddChildLayout(widget);
    }

    void GuiWindow::RemoveWidget(GuiWidget* widget) {
        m_nodes.erase(widget->GetName());
        RemoveChildLayout(widget->GetTag());
    }

	void GuiWindow::Render(void* layoutData) {
        static bool isOpen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("RenderTech Docker", &isOpen, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("RenderTechDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

            static bool layoutSetup = true;
            if (layoutSetup) {
                layoutSetup = false;
                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                for (auto it = m_nodes.begin(); it != m_nodes.end(); it++) {
                    ImGuiID dockId = dockspace_id;
                    if (it->second.direction != GuiSplitDirection::GSD_NONE) {
                        dockId = ImGui::DockBuilderSplitNode(dockspace_id, static_cast<ImGuiDir>(it->second.direction), it->second.ratio, nullptr, &dockspace_id);
                    }
                    ImGui::DockBuilderDockWindow(it->first.c_str(), dockId);
                }

                ImGui::DockBuilderFinish(dockspace_id);
            }
        }

        RenderChildLayouts((void*)0);

        ImGui::End();
    }
}