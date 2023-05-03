#ifndef GUIDOCKER
#define GUIDOCKER

#include "Engine/Core/Render/Gui/GuiLayout.h"
#include "Engine/Core/Render/Gui/GuiWidget.h"

namespace Engine {
	enum class GuiSplitDirection {
		GSD_NONE = -1,
		GSD_LEFT = 0,
		GSD_RIGHT = 1,
		GSD_UP = 2,
		GSD_DOWN = 3
	};

	struct GuiSplitNode {
		GuiSplitDirection direction;
		Float ratio;
	};

	class GuiWindow : public GuiLayout {
	private:
		Map<String, GuiSplitNode> m_nodes;

	public:
		GuiWindow(const String& tag);
		virtual ~GuiWindow() = default;

		void AddWidget(GuiWidget* widget, GuiSplitDirection direction, Float ratio);
		void RemoveWidget(GuiWidget* widget);

	private:
		void Render() override;
	};
}

#endif // !GUIDOCKER
