#include "Engine/Core/System/Resource/Resource.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "Engine/Object/Class/Material.h"

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

namespace Engine {
	Map<String, std::function<void(const Map<String, String>&, Material*)>> xmlNodeProcess = {
		{ "Texture2D", [&](const Map<String, String>& attributes, Material* out) { out->AddTexture(Resource::Load<Texture2D*>(attributes.at("path"))); } },
		{ "Shader", [&](const Map<String, String>& attributes, Material* out) { out->SetShader(Resource::Load<const Array<Int8>>(attributes.at("path"))); } }
	};

	template<>
	Material* Resource::Load(const String& filename) {
		rapidxml::file<>* xmlFile = new rapidxml::file<>(filename.c_str());
		rapidxml::xml_document<>* xmlDocument = new rapidxml::xml_document<>();

		xmlDocument->parse<0>(xmlFile->data());

		rapidxml::xml_node<>* root = xmlDocument->first_node();
		if (std::strcmp(root->name(), "Material") != 0) {
			throw EngineException("[Resource] XML file does not meet material conditions");
		}

		Material* material = ClassType<Material>::CreateObject(ObjectArgument::Dummy());
		for (rapidxml::xml_node<>* node = root->first_node(); node != nullptr; node = node->next_sibling()) {
			Map<String, String> xmlAtributes;
			for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
				xmlAtributes[attr->name()] = attr->value();
			}
			xmlNodeProcess[node->name()](xmlAtributes, material);
		}

		delete xmlDocument;
		delete xmlFile;

		return material;
	}
}