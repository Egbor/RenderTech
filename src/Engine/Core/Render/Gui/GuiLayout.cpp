#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	GuiLayout::GuiLayout(const String& tag) 
		: m_layoutTag(tag), m_childLayouts() {

	}

	void GuiLayout::Render() {
		RenderChildLayouts();
	}

	void GuiLayout::AddChildLayout(GuiLayout* layout) {
		if (layout != nullptr) {
			m_childLayouts[layout->GetTag()] = layout;
		}
	}

	GuiLayout* GuiLayout::GetChildLayout(const String& tag) {
		MapIterator<String, GuiLayout*> it = m_childLayouts.find(tag);
		if (it != m_childLayouts.end()) {
			return it->second;
		}
		return nullptr;
	}

	void GuiLayout::RemoveChildLayout(const String& tag) {
		m_childLayouts.erase(tag);
	}

	String GuiLayout::GetTag() const {
		return m_layoutTag;
	}

	void GuiLayout::RenderChildLayouts() {
		for (auto it = m_childLayouts.begin(); it != m_childLayouts.end(); it++) {
			it->second->Render();
		}
	}

	void GuiLayout::AddOnClickEvent(EventBase<GuiLayout*>& callback) {
		m_eventOnClick += callback;
	}

	void GuiLayout::RemoveOnClickEvent(EventBase<GuiLayout*>& callback) {
		m_eventOnClick -= callback;
	}

	void GuiLayout::InvokeOnClickEvent() {
		m_eventOnClick(this);
	}
}