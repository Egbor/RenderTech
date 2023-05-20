#include "Engine/Rendering/Gui/PropertiesContainer/MeshComponentPropertiesContainer.h"

#include "Engine/Object/Component/MeshComponent.h"

#include <Windows.h>
#include <ShlObj.h>

namespace Engine {
	MeshComponentPropertiesContainer::MeshComponentPropertiesContainer() {
		m_meshButton = new GuiButton("MeshButton", "Set mesh");

		m_meshButton->AddOnClickEvent(Delegate<MeshComponentPropertiesContainer, GuiLayout*>::Allocate(this, &MeshComponentPropertiesContainer::OnClickMeshButton));
	}

	MeshComponentPropertiesContainer::~MeshComponentPropertiesContainer() {
		DELETE_OBJECT(m_meshButton);
	}

	void MeshComponentPropertiesContainer::Attach(GuiLayout* layout, SceneComponent* component) {
		if (component->Is(MeshComponent::TypeIdClass())) {
			MeshComponent* temp = component->As<MeshComponent>();
			layout->AddChildLayout(m_meshButton);
		} else {
			Detach(layout);
		}
	}

	void MeshComponentPropertiesContainer::Detach(GuiLayout* layout) {
		layout->RemoveChildLayout(m_meshButton->GetTag());
	}

	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {

		if (uMsg == BFFM_INITIALIZED) {
			std::string tmp = (const char*)lpData;
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		}

		return 0;
	}

	std::string BrowseFolder(std::string saved_path) {
		TCHAR path[MAX_PATH];

		const char* path_param = saved_path.c_str();

		BROWSEINFO bi = { 0 };
		bi.lpszTitle = ("Browse for folder...");
		bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NONEWFOLDERBUTTON | BIF_STATUSTEXT | BIF_EDITBOX | BIF_VALIDATE;
		bi.lpfn = BrowseCallbackProc;
		bi.lParam = (LPARAM)path_param;

		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

		if (pidl != 0) {
			//get the name of the folder and put it in path
			SHGetPathFromIDList(pidl, path);

			//free memory used
			IMalloc* imalloc = 0;
			if (SUCCEEDED(SHGetMalloc(&imalloc))) {
				imalloc->Free(pidl);
				imalloc->Release();
			}

			return path;
		}

		return "";
	}

	void MeshComponentPropertiesContainer::OnClickMeshButton(GuiLayout* owner) {
		std::string path = BrowseFolder("C:\\");
		if (path.size() > 0) {
			
		}
	}
}