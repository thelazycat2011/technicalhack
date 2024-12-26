#ifndef __GD_H__
#define __GD_H__

#include <cocos2d.h>
#include "stdint.h"
#include "gdMacros.h"

template <class R, class T>
R& from(T base, intptr_t offset) {
	return *reinterpret_cast<R*>(reinterpret_cast<uintptr_t>(base) + offset);
}

namespace gd {
	inline auto base = reinterpret_cast<char*>(GetModuleHandle(0));

	inline bool init() {
		//if the base address is valid, all other values should be valid.
		#if (__WARN_DEBUG__ == true)
		static_assert(sizeof(std::string) == 24,
			"Any classes containing member strings or using strings in functions will break!\n"
			"");
		#endif
		return base;
	}

	struct string {
		union {
			char inline_data[16];
			char* ptr;
		} m_data;
		size_t m_size = 0;
		size_t m_capacity = 15;

		// TODO:
		// ~string() = delete;

		string(const char* data, size_t size) {
			reinterpret_cast<void* (__thiscall*)(void*, const char*, size_t)>(base + 0xa3f0)(this, data, size);
		}

		explicit string(const std::string_view& str) : string(str.data(), str.size()) {}
		// string(const char* str) : string(std::string_view(str)) {}
		string(const std::string& str) : string(str.c_str(), str.size()) {}

		size_t size() const { return m_size; }

		string& operator=(const std::string& other) {
			if (m_capacity > 15) delete m_data.ptr;
			reinterpret_cast<void* (__thiscall*)(void*, const char*, size_t)>(base + 0xa3f0)(this, other.c_str(), other.size());
			return *this;
		}

		const char* c_str() const {
			if (m_capacity <= 15) return m_data.inline_data;
			return m_data.ptr;
		}

		std::string_view sv() const {
			return std::string_view(c_str(), m_size);
		}

		operator std::string() const { return std::string(sv()); }
	};
}

#include "GDProtocols.h"

#include "actions/CCCircleWave.h"

#include "achievement_nodes/AchievementBar.h"
#include "achievement_nodes/AchievementNotifier.h"

#include "layers_scenes_transitions_nodes/FLAlertLayer.h"
#include "layers_scenes_transitions_nodes/GJDropDownLayer.h"
#include "layers_scenes_transitions_nodes/GJListLayer.h"
#include "layers_scenes_transitions_nodes/LoadingLayer.h"
#include "layers_scenes_transitions_nodes/MenuLayer.h"
#include "layers_scenes_transitions_nodes/ScrollingLayer.h"
#include "layers_scenes_transitions_nodes/LoadingCircle.h"
#include "layers_scenes_transitions_nodes/CCBlockLayer.h"
#include "layers_scenes_transitions_nodes/PauseLayer.h"
#include "layers_scenes_transitions_nodes/GJBaseGameLayer.h"
#include "layers_scenes_transitions_nodes/PlayLayer.h"
#include "layers_scenes_transitions_nodes/LevelBrowserLayer.h"
#include "layers_scenes_transitions_nodes/EditLevelLayer.h"
#include "layers_scenes_transitions_nodes/EditorUI.h"
#include "layers_scenes_transitions_nodes/LevelEditorLayer.h"

#include "scroll_nodes/CCContentLayer.h"
#include "scroll_nodes/CCIndexPath.h"
#include "scroll_nodes/CCScrollLayerExt.h"
#include "scroll_nodes/TableView.h"
#include "scroll_nodes/BoomListView.h"
#include "scroll_nodes/CustomListView.h"

#include "level_nodes/GJGameLevel.h"

#include "manager_nodes/GManager.h"
#include "manager_nodes/GameManager.h"
#include "manager_nodes/GameSoundManager.h"
#include "manager_nodes/AchievementManager.h"
#include "manager_nodes/GameLevelManager.h"

#include "menu_nodes/CCMenuItemSpriteExtra.h"
#include "menu_nodes/CCMenuItemToggler.h"

#include "audio_nodes/FMODAudioEngine.h"

#include "sprite_nodes/CCSpritePlus.h"
#include "sprite_nodes/ButtonSprite.h"
#include "sprite_nodes/GameObject.h"
#include "sprite_nodes/CCAnimatedSprite.h"
#include "sprite_nodes/AnimatedShopKeeper.h"
#include "sprite_nodes/PlayerObject.h"
#include "sprite_nodes/TextArea.h"

#include "text_input_nodes/CCTextInputNode.h"

#endif