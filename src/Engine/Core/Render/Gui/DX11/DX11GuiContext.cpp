#include "Engine/Core/Render/Gui/DX11/DX11GuiContext.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

namespace Engine {
    DX11GuiContext::DX11GuiContext(HWND hWnd, DX11Context* dxContext) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(dxContext->GetD3D11Device().Get(), dxContext->GetD3D11Context().Get());
    }
}
