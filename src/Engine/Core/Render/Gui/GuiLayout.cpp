#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	bool IsGuiLayoutExist(Array<GuiLayout*>& layouts, const String& tag, ArrayIterator<GuiLayout*>* outIt = nullptr) {
		ArrayIterator<GuiLayout*> it = std::find_if(layouts.begin(), layouts.end(), [&](GuiLayout* item) {
			return item->GetTag() == tag;
		});

		if (outIt != nullptr) {
			*outIt = it;
		}

		return it != layouts.end();
	}

	GuiLayout::GuiLayout(const String& tag) 
		: m_layoutTag(tag), m_parentLayout(nullptr)
		, m_childLayouts()  {

	}

	void GuiLayout::Render(void* layoutData) {
		RenderChildLayouts(layoutData);
	}

	bool GuiLayout::AddChildLayout(GuiLayout* layout) {
		if (layout != nullptr && !IsGuiLayoutExist(m_childLayouts, layout->GetTag())) {
			layout->Register(this);
			m_childLayouts.push_back(layout);
			return true;
		}
		return false;
	}

	bool GuiLayout::RemoveChildLayout(const String& tag, GuiLayout** outLayout) {
		ArrayIterator<GuiLayout*> it;
		if (IsGuiLayoutExist(m_childLayouts, tag, &it)) {
			(*it)->UnRegister();
			m_childLayouts.erase(it);

			if (outLayout != nullptr) {
				*outLayout = *it;
			}

			return true;
		}
		return false;
	}

	String GuiLayout::GetTag() const {
		return m_layoutTag;
	}

	void GuiLayout::RenderChildLayouts(void* layoutData) {
		for (auto it = m_childLayouts.begin(); it != m_childLayouts.end(); it++) {
			(*it)->Render(layoutData);
		}
	}

	void GuiLayout::AddOnClickEvent(EventBase<GuiLayout*>& callback) {
		m_eventOnClick += callback;
	}

	void GuiLayout::RemoveOnClickEvent(EventBase<GuiLayout*>& callback) {
		m_eventOnClick -= callback;
	}

	void GuiLayout::Register(GuiLayout* parent) {
		m_parentLayout = parent;
	}

	void GuiLayout::UnRegister() {
		m_parentLayout = nullptr;
	}
}