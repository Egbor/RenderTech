#include "Engine/Core/Render/Gui/DX11/DX11GuiContext.h"
#include "Engine/Core/Render/Api/DX11/DX11Texture.h"

#include "Engine/Core/System/Exception/EngineException.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Engine {
    LRESULT CALLBACK HookWndProcForDevGui(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode < 0) {
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        }

        switch (nCode) {
        case HC_ACTION:
        {
            //LPMSG wndProcArgs = (LPMSG)lParam;
            //if (ImGui_ImplWin32_WndProcHandler(wndProcArgs->hwnd, wndProcArgs->message, wndProcArgs->wParam, wndProcArgs->lParam)) {
            //    return CallNextHookEx(NULL, nCode, wParam, lParam);
            //}
            break;
        }
        default:
            break;
        }

        return CallNextHookEx(NULL, nCode, wParam, lParam);
    }

    DX11GuiContext::DX11GuiContext(HWND hWnd, DX11Context* dxContext) 
        : m_hWnd(hWnd), m_dxContext(dxContext) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplWin32_Init(hWnd);
        ImGui_ImplDX11_Init(dxContext->GetD3D11Device().Get(), dxContext->GetD3D11Context().Get());

        //SetWindowsHookExA(WH_GETMESSAGE, HookWndProcForDevGui, NULL, GetCurrentThreadId());
    }

    DX11GuiContext::~DX11GuiContext() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void DX11GuiContext::PrepareLayout() {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void DX11GuiContext::Draw() {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // It is necessary if used ImGuiConfigFlags_ViewportsEnable
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    Context* DX11GuiContext::GetContext() const {
        return m_dxContext;
    }

    void DX11GuiContext::AllocateGuiImageResource(Texture2D* texture, GuiResourceId* resourceId) {
        if ((*resourceId) != nullptr) {
            FreeGuiResource(resourceId);
        }
        
        if (texture != nullptr) {
            HRESULT hr = 0;
            ComPtr<ID3D11Device> d3dDevice = m_dxContext->GetD3D11Device();
            IDX11Texture2D* dxTexture2D = dynamic_cast<IDX11Texture2D*>(texture);
            ID3D11ShaderResourceView** d3dView = reinterpret_cast<ID3D11ShaderResourceView**>(resourceId);

            D3D11_SHADER_RESOURCE_VIEW_DESC d3dShaderResourceViewDesc;
            dxTexture2D->GetD3D11ResourceDesc(d3dShaderResourceViewDesc);

            if (FAILED(hr = d3dDevice->CreateShaderResourceView(dxTexture2D->Data().GetD3D11Texture2D().Get(),
                &d3dShaderResourceViewDesc, d3dView))) {
                throw EngineException("[DX11GuiContext] ID3D11Device::CreateShaderResourceView() failed");
            }
        }
    }

    void DX11GuiContext::FreeGuiResource(GuiResourceId* resourceId) {
        IUnknown* comptr = reinterpret_cast<IUnknown*>(*(resourceId));
        comptr->Release();
        comptr = nullptr;
    }

    void DX11GuiContext::Close() {
        PostMessage(m_hWnd, WM_DESTROY, NULL, NULL);
    }
}
