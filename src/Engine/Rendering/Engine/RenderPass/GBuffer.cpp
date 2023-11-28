#include "Engine/Rendering/Engine/RenderPass/GBuffer.h"

namespace Engine {
	GBuffer::GBuffer() 
		: m_targets() {

	}

	GBuffer::~GBuffer() {
		DELETE_ARRAY_OF_OBJECTS(m_targets);
	}

	Int32 GBuffer::Attach(ITargetResourceData* resource) {
		m_targets.push_back(resource);
		return static_cast<Int32>(m_targets.size() - 1);
	}

	Int32 GBuffer::Attach(TextureType type, TextureFormat format, Int32 width, Int32 height) {
		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		return this->Attach(factory->CreateTarget(TextureType::TT_DEFAULT, TextureFormat::TF_R8G8B8A8_BMP, width, height));
	}

	void GBuffer::Clear() {
		for (Size i = 0; i < m_targets.size(); i++) {
			m_targets[i]->Clear(Core::GetInstance()->GetContext());
		}
	}

	const Array<ITargetResourceData*>& GBuffer::GetTargets() const {
		return m_targets;
	}

	const Array<ITextureResourceData*> GBuffer::GetTextures() const {
		Array<ITextureResourceData*> result(m_targets.size());
		for (Size i = 0; i < result.size(); i++) {
			result[i] = m_targets[i]->GetTextureResource();
		}
		return result;
	}

	UBuffer::UBuffer() 
		: m_buffers() {

	}

	UBuffer::~UBuffer() {
		DELETE_ARRAY_OF_OBJECTS(m_buffers);
	}


	Int32 UBuffer::Attach(Int32 bufferSize) {
		Array<Int8> dummy(bufferSize);

		IRenderResourceFactory* factory = Core::GetInstance()->GetContext()->QueryResourceFactory();
		IBufferResourceData* resource = factory->CreateBuffer(BufferType::BT_UNIFORM, 1, dummy.size(), &dummy[0]);

		m_buffers.push_back(resource);
		return static_cast<Int32>(m_buffers.size() - 1);
	}

	void UBuffer::Update(Int32 index) {
		IDynamicResourceData* buffer = dynamic_cast<IDynamicResourceData*>(m_buffers[index]);
		buffer->Update(Core::GetInstance()->GetContext());
	}

	RawData UBuffer::GetBufferData(Int32 index) const {
		IDynamicResourceData* buffer = dynamic_cast<IDynamicResourceData*>(m_buffers[index]);
		return buffer->GetBufferData();
	}

	const Array<IBufferResourceData*>& UBuffer::GetBuffers() const {
		return m_buffers;
	}
}