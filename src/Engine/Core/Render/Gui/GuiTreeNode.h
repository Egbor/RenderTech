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
		GuiTree* m_selected;

		Event<GuiLayout*, GuiTree*> m_eventOnSelected;

	public:
		GuiTree(const String& tag, const String& label, void* id);
		virtual ~GuiTree() = default;

		bool AddTreeNode(GuiTree* node);
		bool RemoveTreeNode(const String& tag, GuiTree** outNode = nullptr);

		void AddOnSelectedEvent(EventBase<GuiLayout*, GuiTree*>& callback);
		void RemoveOnSelectedEvent(EventBase<GuiLayout*, GuiTree*>& callback);

	private:
		void OnNodeClicked(GuiLayout* owner);
		void OnNodeSelected(GuiLayout* owner, GuiTree* node);
	};
}

#endif // !GUITREENODE
