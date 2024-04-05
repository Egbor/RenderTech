#ifndef IDX11_SELF_BINDABLE_H
#define IDX11_SELF_BINDABLE_H

#include "Engine/Core/Render/Api/DX11/DX11Def.h"

namespace Engine {
	class IDX11SelfBindable {
	public:
		virtual ~IDX11SelfBindable() = default;
		virtual void Bind(ComPtr<ID3D11DeviceContext> d3dContext) = 0;
	};
}

#endif // !IDX11_SELF_BINDABLE_H
