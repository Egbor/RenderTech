#ifndef GUITREENODE
#define GUITREENODE

#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	class GuiTreeNode : public GuiLayout {
	private:
		String m_label;
		void* m_id;

		bool m_isSelected;

	public:
		GuiTreeNode(const String& tag, const String& label, void* id);
		virtual ~GuiTreeNode() = default;

		void SetActive(bool value);
		bool GetActive() const;
		void* GetId() const;

	private:
		void Render(void* layoutData) override;
	};

	class GuiTree : public GuiTreeNode {
	private:
		GuiTreeNode* m_selected;

		Event<GuiLayout*, GuiTreeNode*> m_eventOnSleceted;

	public:
		GuiTree(const String& tag, const String& label, void* id);
		virtual ~GuiTree() = default;

		bool AddTreeNode(GuiTreeNode* node);
		bool RemoveTreeNode(const String& tag, GuiTreeNode** outNode = nullptr);

		void AddOnSelectedEvent(EventBase<GuiLayout*, GuiTreeNode*>& callback);
		void RemoveOnSelectedEvent(EventBase<GuiLayout*, GuiTreeNode*>& callback);

	private:
		void OnNodeClicked(GuiLayout* owner);
	};
}

#endif // !GUITREENODE
