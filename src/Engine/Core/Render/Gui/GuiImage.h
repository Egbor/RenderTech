#ifndef GUIIMAGE
#define GUIIMAGE

#include "Engine/Core/Render/Gui/GuiLayout.h"
#include "Engine/Core/Render/Gui/GuiContext.h"
#include "Engine/Object/Class/Target.h"

namespace Engine {
	class GuiImage : public GuiLayout {
	private:
		GuiContext* m_context;
		Texture2D* m_texture;

	public:
		GuiImage(const String& tag, GuiContext* context, Int32 width, Int32 height, TextureFormat format);
		virtual ~GuiImage() = default;

		Int32 GetWidth() const;
		Int32 GetHeight() const;

		void Resize(Int32 width, Int32 height, TextureFormat format);

		Texture2D* GetTarget() const;

	private:
		void Render() override;
	};
}

#endif // !GUIIMAGE
