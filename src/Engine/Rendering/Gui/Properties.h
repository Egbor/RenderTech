#ifndef PROPERTIES
#define PROPERTIES

#include "Engine/Core/Render/Gui/GuiWidget.h"
#include "Engine/Core/Render/Gui/GuiDragInput.h"

#include "Engine/Object/Entity/Entity.h"

namespace Engine {
	class Properties : public GuiWidget {
	private:
		GuiDragInput* m_position;
		GuiDragInput* m_rotation;
		GuiDragInput* m_scale;

	public:
		Properties(const String& name, const String& tag = "Properties");
		virtual ~Properties();

		void SetEntity(Entity* entity);
	};
}

#endif // !PROPERTIES
