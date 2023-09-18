#include "Engine/Core/System/Import/Import.h"

namespace Engine {
	Importer::Importer(const String& filename) 
		: m_filename(filename) {

	}

	const String& Importer::GetFilename() const {
		return m_filename;
	}

	void Importer::ImportTo(Object* object) {
		if (object == nullptr) {
			throw EngineException("[Importer::ImportTo()] object is null");
		}
	}
}