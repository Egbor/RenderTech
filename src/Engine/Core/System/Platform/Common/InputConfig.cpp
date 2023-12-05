#include "Engine/Core/System/Platform/Common/InputConfig.h"
#include "Engine/Core/System/Exception/EngineException.h"

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"

namespace Engine {
	InputKey ConvertStringToKey(const String& key) {
		static Map<String, InputKey> keytable = {
			{ "a", InputKey::IK_A },
			{ "c", InputKey::IK_C },
			{ "b", InputKey::IK_B },
			{ "d", InputKey::IK_D },
			{ "e", InputKey::IK_E },
			{ "f", InputKey::IK_F },
			{ "g", InputKey::IK_G },
			{ "h", InputKey::IK_H },
			{ "i", InputKey::IK_I },
			{ "j", InputKey::IK_J },
			{ "k", InputKey::IK_K },
			{ "l", InputKey::IK_L },
			{ "m", InputKey::IK_M },
			{ "n", InputKey::IK_N },
			{ "o", InputKey::IK_O },
			{ "p", InputKey::IK_P },
			{ "q", InputKey::IK_Q },
			{ "r", InputKey::IK_R },
			{ "s", InputKey::IK_S },
			{ "t", InputKey::IK_T },
			{ "u", InputKey::IK_U },
			{ "v", InputKey::IK_V },
			{ "w", InputKey::IK_W },
			{ "x", InputKey::IK_X },
			{ "y", InputKey::IK_Y },
			{ "x", InputKey::IK_X },
			{ "mouse lbutton", InputKey::IK_MOUSE_BUTTON_LEFT },
			{ "mouse rbutton", InputKey::IK_MOUSE_BUTTON_RIGHT },
			{ "mouse mbutton", InputKey::IK_MOUSE_BUTTON_MIDDLE },
			{ "mouse xbutton1", InputKey::IK_MOUSE_XBUTTON1 },
			{ "mouse xbutton2", InputKey::IK_MOUSE_XBUTTON2 },
			{ "mouse wheel", InputKey::IK_MOUSE_WHEEL },
			{ "mouse x", InputKey::IK_MOUSE_X },
			{ "mouse y", InputKey::IK_MOUSE_Y },
			{ "gamepad left x", InputKey::IK_GAMEPAD_THUMB_LEFT_X },
			{ "gamepad left y", InputKey::IK_GAMEPAD_THUMB_LEFT_Y },
			{ "gamepad right x", InputKey::IK_GAMEPAD_THUMB_RIGHT_X },
			{ "gamepad right y", InputKey::IK_GAMEPAD_THUMB_RIGHT_Y },
			{ "gamepad ltrigger", InputKey::IK_GAMEPAD_TRIGGER_LEFT },
			{ "gamepad rtrigger", InputKey::IK_GAMEPAD_TRIGGER_RIGHT },
			{ "gamepad up", InputKey::IK_GAMEPAD_DPAD_UP },
			{ "gamepad down", InputKey::IK_GAMEPAD_DPAD_DOWN },
			{ "gamepad left", InputKey::IK_GAMEPAD_DPAD_LEFT },
			{ "gamepad right", InputKey::IK_GAMEPAD_DPAD_RIGHT },
			{ "gamepad start", InputKey::IK_GAMEPAD_START },
			{ "gamepad back", InputKey::IK_GAMEPAD_BACK },
			{ "gamepad lbutton", InputKey::IK_GAMEPAD_LEFT_THUMB },
			{ "gamepad rbutton", InputKey::IK_GAMEPAD_RIGHT_THUMB },
			{ "gamepad lshoulder", InputKey::IK_GAMEPAD_LEFT_SHOULDER },
			{ "gamepad rshoulder", InputKey::IK_GAMEPAD_RIGHT_SHOULDER },
			{ "gamepad a", InputKey::IK_GAMEPAD_A },
			{ "gamepad b", InputKey::IK_GAMEPAD_B },
			{ "gamepad x", InputKey::IK_GAMEPAD_X },
			{ "gamepad y", InputKey::IK_GAMEPAD_Y },
		};

		String copyOfString = key;
		std::transform(copyOfString.begin(), copyOfString.end(), copyOfString.begin(), std::tolower);
		return keytable.at(copyOfString);
	}

	template<class TBindValue>
	TBindValue ConvertStringToValue(const String& value);

	template<>
	InputEvent ConvertStringToValue(const String& value) {
		static Map<String, InputEvent> valuetable = {
			{ "released", InputEvent::IE_RELEASED },
			{ "pressed", InputEvent::IE_PRESSED },
		};

		String copyOfString = value;
		std::transform(copyOfString.begin(), copyOfString.end(), copyOfString.begin(), std::tolower);
		return valuetable.at(copyOfString);
	}

	template<>
	Float ConvertStringToValue(const String& value) {
		return std::stof(value);
	}

	template<class TBindValue>
	void ProcessBinding(const String& key, const String& value, Array<InputBinding<TBindValue>>& out) {
		out.push_back({ ConvertStringToKey(key), ConvertStringToValue<TBindValue>(value) });
	}

	template<class TBindValue>
	void ProcessBindingNodes(rapidxml::xml_node<>* root, Map<String, Array<InputBinding<TBindValue>>>& out) {
		for (rapidxml::xml_node<>* node = root; node != nullptr; node = node->next_sibling()) {
			Map<String, String> xmlAttributes;
			for (rapidxml::xml_attribute<>* attr = node->first_attribute(); attr != nullptr; attr = attr->next_attribute()) {
				xmlAttributes[attr->name()] = attr->value();
			}
			if (!xmlAttributes.empty()) {
				ProcessBinding<TBindValue>(xmlAttributes.at("key"), xmlAttributes.at("value"), out[xmlAttributes.at("id")]);
			}
		}
	}

	InputConfig::InputConfig(const String& filename) {
		rapidxml::file<>* xmlFile = new rapidxml::file<>(filename.c_str());
		rapidxml::xml_document<>* xmlDocument = new rapidxml::xml_document<>();

		xmlDocument->parse<0>(xmlFile->data());

		rapidxml::xml_node<>* root = xmlDocument->first_node();
		if (std::strcmp(root->name(), "Input") != 0) {
			throw EngineException("[InputConfig] XML file does not meet input conditions");
		}

		for (rapidxml::xml_node<>* node = root->first_node(); node != nullptr; node = node->next_sibling()) {
			if (std::strcmp(node->name(), "Actions") == 0) {
				ProcessBindingNodes(node->first_node(), m_actions);
			} else if (std::strcmp(node->name(), "Axis") == 0) {
				ProcessBindingNodes(node->first_node(), m_axis);
			}
		}

		delete xmlDocument;
		delete xmlFile;
	}

	const Array<InputBinding<InputEvent>>& InputConfig::GetActions(const String& tag) const {
		return m_actions.at(tag);
	}

	const Array<InputBinding<Float>>& InputConfig::GetAxis(const String& tag) const {
		return m_axis.at(tag);
	}
}