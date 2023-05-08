#ifndef PROPERTIES
#define PROPERTIES

#include "Engine/Core/Render/Gui/GuiWidget.h"
#include "Engine/Core/Render/Gui/GuiDragInput.h"
#include "Engine/Core/Render/Gui/GuiColorPicker.h"

#include "Engine/Object/Entity/Entity.h"

namespace Engine {
	class Properties : public GuiWidget {
	private:
		GuiDragInput* m_position;
		GuiDragInput* m_rotation;
		GuiDragInput* m_scale;

		GuiColorPicker* m_color;

	public:
		Properties(const String& name, const String& tag = "Properties");
		virtual ~Properties();

		void SetComponent(SceneComponent* component);
	};
}

#endif // !PROPERTIES
