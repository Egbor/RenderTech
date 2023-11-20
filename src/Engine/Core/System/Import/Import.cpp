#include "Engine/Core/System/Import/Import.h"

namespace Engine {
	LoaderBase::LoaderBase(const String& filename) 
		: m_filename(filename) {

	}

	const String& LoaderBase::GetFilename() const {
		return m_filename;
	}

	AbstractImporter::AbstractImporter(const String& filename)
		: LoaderBase(filename) {

	}

	AbstractLoader::AbstractLoader(const String& filename)
		: LoaderBase(filename) {

	}
}