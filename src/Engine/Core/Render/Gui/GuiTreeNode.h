#ifndef GUITREENODE
#define GUITREENODE

#include "Engine/Core/Render/Gui/GuiLayout.h"

namespace Engine {
	class GuiTreeNode : public GuiLayout {
	private:
		String m_label;
		const void* m_id;

	public:
		GuiTreeNode(const String& tag, const String& label, const void* id);
		virtual ~GuiTreeNode() = default;

	private:
		void Render(void* layoutData) override;
	};
}

#endif // !GUITREENODE
