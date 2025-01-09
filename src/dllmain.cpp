#include "includes.h"
#include <fstream>
#include "matplist.hpp"
#include "impl.hpp"
#include "matnicehackutil.hpp"
#include <nfd.h>
using namespace gd;
const int param_count = 15;
bool noclip = false,
	safeMode = false,
	autoSafeMode = true,
    visible = false,
    restartButton = false,
    fpsBypass = false,
	speedhack = false,
	verifyHack = false,
	practiceMusic = false,
	unlockIcon = false,
	objectBypass = false,
	copyHack = false,
	cheatIndicator = false,
	editLevel = false,
	wafflixMode = false;

float fps = 240.0f,
	speed = 1.0f;

template <typename T2>

void patch(uint32_t address, T2 data)
{
	DWORD old;
	VirtualProtect(reinterpret_cast<void*>(address), sizeof(T2), PAGE_EXECUTE_READWRITE, &old);
	memcpy(reinterpret_cast<void*>(address), &data, sizeof(T2));
	VirtualProtect(reinterpret_cast<void*>(address), sizeof(T2), old, &old);
}

// Better way patching
void sequence_patch(uint32_t address, std::vector<uint8_t> data) {
	for (uint32_t i = 0; i < data.size(); i++) patch(address + i, (uint8_t)data[i]);
}

bool isCheating() {
	// TODO : Make this thing true even after you turn it off, unless you restart
	return noclip || (speedhack && speed != 1.0f);
}

// Patching
void update_fps_bypass() {
	float value = (fpsBypass && fps > 0) ? fps : 60.0f;
	CCApplication::sharedApplication()->setAnimationInterval(1.0 / value);
}
void update_speedhack() {
	float value = (speedhack && speed > 0) ? speed : 1.0f;
	Speedhack::SetSpeed(value);
}
void patchRestart() {if (restartButton) patch(0x55BC96, (uint16_t)0xC039); else patch(0x55BC96, (uint16_t)0xF883);}
void patchVerifyHack() {if (verifyHack) patch(0x4594E5, (uint8_t)0xEB); else patch(0x4594E5, (uint8_t)0x75);}
void patchPracticeMusic() { //thanks pololak for this
	auto gdbase = GetModuleHandleA(0);
	if (practiceMusic) {
        sequence_patch((uint32_t)gdbase + 0x17b765, {0x90, 0x90});
        sequence_patch((uint32_t)gdbase + 0x17b797, {0x90, 0x90});
        sequence_patch((uint32_t)gdbase + 0x17d0f5, {0x90, 0x90, 0x90, 0x90, 0x90, 0x90});
        sequence_patch((uint32_t)gdbase + 0x17d953, {0x90, 0x90});
        sequence_patch((uint32_t)gdbase + 0x17d981, {0x90, 0x90, 0x90, 0x90, 0x90});
    }
    else {
        sequence_patch((uint32_t)gdbase + 0x17b765, {0x75, 0x3E});
        sequence_patch((uint32_t)gdbase + 0x17b797, {0x75, 0x0C});
        sequence_patch((uint32_t)gdbase + 0x17d0f5, {0x0F, 0x85, 0x53, 0x02, 0x00, 0x00});
        sequence_patch((uint32_t)gdbase + 0x17d953, {0x75, 0x41});
        sequence_patch((uint32_t)gdbase + 0x17d981, {0xE8, 0x2A, 0x3F, 0xEA, 0xFF});
    }
}
void patchObjectCount() {
	if (objectBypass) {
		patch(0x469910, (uint8_t)0xEB);
		patch(0x46B89A, (uint8_t)0xEB);
		patch(0x46BBFE, (uint8_t)0xEB);
		patch(0x4EF1A7, (uint8_t)0xEB);
	}
	else {
		patch(0x469910, (uint8_t)0x74);
		patch(0x46B89A, (uint8_t)0x7C);
		patch(0x46BBFE, (uint8_t)0x7C);
		patch(0x4EF1A7, (uint8_t)0x75);
	}
}
void patchCopyHack() {
	if (copyHack) sequence_patch(0x5083C0, {0xE9, 0x01, 0x01, 0x00, 0x00, 0x90});
	else sequence_patch(0x5083C0, {0x0F, 0x84, 0x00, 0x01, 0x00, 0x00});
}

void patchEditLevel() {
	if (editLevel) sequence_patch(0x55BAAF, {0x90, 0x90});
	else sequence_patch(0x55BAAF, {0x75, 0x62});
}

// Icon Hack
bool(__thiscall* GameManager_isIconUnlockedO)(GameManager*, int, int); // also thanks to pololak
bool __fastcall GameManager_isIconUnlockedH(GameManager* self, void* edx, int id, int type) {
    if (unlockIcon) return true;
    else return GameManager_isIconUnlockedO(self, id, type);
}

bool(__thiscall* GameManager_isColorUnlocked)(GameManager*, int, unsigned int); // also thanks to pololak
bool __fastcall GameManager_isColorUnlocked_H(GameManager* self, void* edx, int id, unsigned int type) {
    if (unlockIcon) return true;
    else return GameManager_isColorUnlocked(self, id, type);
}

// Noclip
void (__thiscall* PlayLayer_destroyPlayer)(CCNode*, int*);
void __fastcall PlayLayer_destroyPlayer_H(CCNode* self, void* edx, int* idk) {
	if (!noclip) PlayLayer_destroyPlayer(self, idk);
}

bool (__thiscall* PlayLayer_init)(CCNode*, int*);
bool __fastcall PlayLayer_init_H(CCNode* self, void* edx, int* param2) {
	auto chIn = CCLabelBMFont::create(".", "bigFont.fnt");
	chIn->setPosition({CCDirector::sharedDirector()->getScreenLeft() + 8, CCDirector::sharedDirector()->getScreenTop() + 2});
	chIn->setZOrder(999999);
	chIn->setOpacity(255);
	chIn->setTag(69420);
	self->addChild(chIn);
	if (!PlayLayer_init(self, param2)) return false;
	return true;
}

template <class S>
void dump_level(GJGameLevel* level, S& stream) {
	const auto song_key = level->m_songID ?
		format("<k>k45</k><i>{}</i>", level->m_songID) : 
			level->m_audioTrack ?
				format("<k>k8</k><i>{}</i>", level->m_audioTrack) : "";
	// encode it twice because gdshare does too
	// why? i dont know fod is stupid
	const auto encoded_desc = base64_encode(base64_encode(level->m_levelDesc.sv()));
	format_to(stream, R"(<d>
	<k>kCEK</k><i>4</i>
	<k>k2</k><s>{}</s>
	<k>k3</k><s>{}</s>
	<k>k4</k><s>{}</s>
	{}
	<k>k13</k><t/>
	<k>k21</k><i>2</i>
	<k>k50</k><i>24</i>
</d>)", level->m_levelName.sv(), encoded_desc, level->m_levelString.sv(), song_key);
}

template <class S>
GJGameLevel* import_level(S& stream) {
	auto data = matplist::parse(stream);
	GJGameLevel* level = GameLevelManager::sharedState()->createNewLevel();
	for (auto& [key, var] : data) {
		if (!std::holds_alternative<matplist::Value>(var)) continue;
		matplist::Value value = std::get<matplist::Value>(var);
		using namespace std::literals;
		if (key == "k2"sv) {
			level->m_levelName = value.value;
		} else if (key == "k3"sv) {
			level->m_levelDesc = base64_decode(base64_decode(value.value));
		} else if (key == "k4"sv) {
			level->m_levelString = value.value;
		} else if (key == "k8"sv) {
			const auto text = value.value;
			level->m_audioTrack = std::stoi(text);
		} else if (key == "k45"sv) {
			const auto text = value.value;
			level->m_songID = std::stoi(text);
		}
	}
	return level;
}

bool (__thiscall* EditLevelLayer_init)(EditLevelLayer* self, GJGameLevel* level);
bool __fastcall EditLevelLayer_init_H(EditLevelLayer* self, void* edx, GJGameLevel* level) {
	if (!EditLevelLayer_init(self, level)) return false;
	auto menu = CCMenu::create();
	constexpr auto handler = [](CCObject* self, CCObject*) {
		auto* const level = reinterpret_cast<EditLevelLayer*>(self)->level();
		nfdchar_t* path = nullptr;
		if (NFD_SaveDialog("gmd", nullptr, &path) == NFD_OKAY) {
			std::ofstream file(path);
			dump_level(level, file);
			free(path);
			auto alert = FLAlertLayer::create(nullptr, "Success", "The level has been saved", "OK", nullptr, 320.f, false, 0);
			alert->show();
		}
	};
	auto button = CCMenuItemSpriteExtra::create(
		ButtonSprite::create("export", 40, 0, 0.5f, false, "goldFont.fnt", "GJ_button_01.png", 30),
		nullptr, self, to_handler<SEL_MenuHandler, handler>
	);
	menu->setZOrder(1);
	menu->setPosition(35, 69);
	menu->addChild(button);
	self->addChild(menu);
	return true;
}

bool (__thiscall* LevelBrowserLayer_init)(LevelBrowserLayer* self, GJSearchObject* obj);
bool __fastcall LevelBrowserLayer_init_H(LevelBrowserLayer* self, void* edx, GJSearchObject* obj) {
	if (!LevelBrowserLayer_init(self, obj)) return false;
	if (obj->m_type == SearchType::MyLevels) {
		auto menu = CCMenu::create();
		menu->setPosition(CCDirector::sharedDirector()->getWinSize().width - 30.f, 85);
		self->addChild(menu);
		constexpr auto handler = [](CCObject*, CCObject*) {
			nfdchar_t* path = nullptr;
			if (NFD_OpenDialog("gmd", nullptr, &path) == NFD_OKAY) {
				std::ifstream file(path);
				
				GJGameLevel* level = import_level(file);
				free(path);
				if (!level) {
					FLAlertLayer::create(nullptr, "Error", "Failed to import", "OK", nullptr, 320.f, false, 0)->show();
					return;
				}
				auto scene = EditLevelLayer::scene(level);
				// CCDirector::sharedDirector()->pushScene(scene);
			}
		};

		auto button = CCMenuItemSpriteExtra::create(
			ButtonSprite::create("import", 40, 0, 0.5f, false, "goldFont.fnt", "GJ_button_01.png", 30),
			nullptr, self, to_handler<SEL_MenuHandler, handler>
		);
		menu->setZOrder(1);
		menu->addChild(button);
	}

	return true;
}


// Idk
void (__thiscall* PlayLayer_update)(CCNode*, float);
void __fastcall PlayLayer_update_H(CCNode* self, void* edx , float idk) {
	PlayLayer_update(self, idk);
	CCNode* chIn = self->getChildByTag(69420);
	CCLabelBMFont* chInAsTag = (CCLabelBMFont*)chIn;
	if (cheatIndicator) {
		chInAsTag->setOpacity(255);
		if (isCheating()) chInAsTag->setColor({255, 0, 0});
		else chInAsTag->setColor({0, 255, 0});
	}
	else chInAsTag->setOpacity(0);
}

// Safe mode
void (__fastcall* PlayLayer_levelComplete)(int*);
void __fastcall PlayLayer_levelComplete_H(int* idk) {
	if (safeMode || (autoSafeMode && isCheating())) reinterpret_cast<void(__fastcall*)(CCNode *)>(0x57DE80)((CCNode*) idk);
	else PlayLayer_levelComplete(idk);
}

void load_config() { // Yandere Simulator's code
	freopen("TechnicalHack.txt", "r", stdin);
	int hackcount;
	std::cin >> hackcount;
	for (int i = 0; i < hackcount; i++) {
		std::string name;
		std::cin >> name;
		if (name == "speedhack") std::cin >> speedhack;
		if (name == "speed") std::cin >> speed;
		if (name == "fpsbypass") std::cin >> fpsBypass;
		if (name == "fps") std::cin >> fps;
		if (name == "noclip") std::cin >> noclip;
		if (name == "safemode") std::cin >> safeMode;
		if (name == "autosafemode") std::cin >> autoSafeMode;
		if (name == "practicemusicsync") std::cin >> practiceMusic;
		if (name == "unlockicon") std::cin >> unlockIcon;
		if (name == "restartbutton") std::cin >> restartButton;
		if (name == "verifyhack") std::cin >> verifyHack;
		if (name == "objectbypass") std::cin >> objectBypass;
		if (name == "copyhack") std::cin >> copyHack;
		if (name == "cheatindicator") std::cin >> cheatIndicator;
		if (name == "editlevel") std::cin >> editLevel;
	}

	// Even worse version of Yandere Simulator's code
	update_fps_bypass();
	update_speedhack();
	patchObjectCount();
	patchPracticeMusic();
	patchRestart();
	patchVerifyHack();
	patchCopyHack();
	patchEditLevel();
}

void save_config() { // Yandere Simulator's code 2
	freopen("TechnicalHack.txt", "w", stdout);
	std::cout << param_count << std::endl;
	std::cout << "speedhack " << speedhack << std::endl;
	std::cout << "speed " << speed << std::endl;
	std::cout << "fpsbypass " << fpsBypass << std::endl;
	std::cout << "fps " << fps << std::endl;
	std::cout << "noclip " << noclip << std::endl;
	std::cout << "safemode " << safeMode << std::endl;
	std::cout << "autosafemode " << autoSafeMode << std::endl;
	std::cout << "practicemusicsync " << practiceMusic << std::endl;
	std::cout << "unlockicon " << unlockIcon << std::endl;
	std::cout << "restartbutton " << restartButton << std::endl;
	std::cout << "verifyhack " << verifyHack << std::endl;
	std::cout << "objectbypass " << objectBypass << std::endl;
	std::cout << "copyhack " << copyHack << std::endl;
	std::cout << "cheatindicator " << cheatIndicator << std::endl;
	std::cout << "editlevel " << editLevel << std::endl;
}

//render func
void imgui_render() {
	const auto& font = ImGui::GetIO().Fonts->Fonts.back();
	ImGui::PushFont(font);
	if (visible) {

		// ImGui::ShowDemoWindow();
		// auto frame_size = CCDirector::sharedDirector()->getOpenGLView()->getFrameSize();

		constexpr float border = 25;
		ImGui::SetNextWindowPos({ border, border });
		// ImGui::SetNextWindowSizeConstraints({0, 0}, {frame_size.width, frame_size.height - border * 2.f});
		ImGui::SetNextWindowBgAlpha(0.75f);

		if (ImGui::Begin("TechnicalHack", nullptr,
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)) {
			ImGui::SetNextItemWidth(120);
			if (ImGui::InputFloat("##fpsbypass", &fps)) update_fps_bypass();
			ImGui::SameLine();
			if (ImGui::Checkbox("FPS bypass", &fpsBypass)) update_fps_bypass();
			ImGui::SetNextItemWidth(120);
			if (ImGui::InputFloat("##speedhack", &speed)) update_speedhack();
			ImGui::SameLine();
			if (ImGui::Checkbox("Speedhack", &speedhack)) update_speedhack();
			ImGui::Checkbox("Noclip", &noclip);
			ImGui::Checkbox("Safe Mode", &safeMode);
			ImGui::Checkbox("Auto Safe Mode", &autoSafeMode);
            if (ImGui::Checkbox("Restart Button", &restartButton)) patchRestart();
			if (ImGui::Checkbox("Edit Level", &editLevel)) patchEditLevel();
			if (ImGui::Checkbox("Verify Hack", &verifyHack)) patchVerifyHack();
			ImGui::Checkbox("Unlock Icon", &unlockIcon);
			if (ImGui::Checkbox("Practice Music Sync", &practiceMusic)) patchPracticeMusic();
			if (ImGui::Checkbox("Copy Hack", &copyHack)) patchCopyHack();
			if (ImGui::Checkbox("Object Bypass", &objectBypass)) patchObjectCount();
			ImGui::Checkbox("Cheat Indicator", &cheatIndicator);
			// if (ImGui::Checkbox("Wafflix Mode", &practiceMusic)) patchPracticeMusic();
			if (ImGui::Button("Save config")) save_config();
        }

        ImGui::PopFont();
    }
	// Alt-tab/Unfocus/Hide the games windows apperently set the FPS back to 60
	// This is designed to counter that
	if (fpsBypass && (CCDirector::sharedDirector()->getAnimationInterval() != 1 / fps)) update_fps_bypass();
}

void imgui_init() {
	ImGui::GetIO().Fonts->AddFontFromFileTTF("Muli-SemiBold.ttf", 18.f);

	auto& style = ImGui::GetStyle();
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowBorderSize = 0;
	style.WindowRounding = 10;
	style.FrameRounding = 10;
	style.ChildRounding = 10;

	auto colors = style.Colors;
	colors[ImGuiCol_FrameBg] = ImVec4(0.31f, 0.31f, 0.31f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.59f, 0.59f, 0.59f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.61f, 0.61f, 0.61f, 0.67f);
	colors[ImGuiCol_TitleBg] = colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.2f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.89f, 0.89f, 0.89f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.71f, 0.71f, 0.71f, 0.35f);
}

DWORD WINAPI thread_func(void* hModule) {
    // initialize minhook
    MH_Initialize();
    auto base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
	Speedhack::Setup();
    ImGuiHook::setToggleCallback([]() { visible = !visible; });
	ImGuiHook::setRenderFunction(imgui_render);
	ImGuiHook::setInitFunction(imgui_init);

	ImGuiHook::setupHooks([](auto addr, auto hook, auto orig) {
		MH_CreateHook(addr, hook, orig);
		MH_EnableHook(addr);
	});
	
	load_config();
	sequence_patch(0x570FC2, {0x90, 0x90, 0x90, 0x90, 0x90}); // Patch speedhack anticheat
	Hook(base + 0x9AFC0, GameManager_isIconUnlockedH, GameManager_isIconUnlockedO);
	Hook(base + 0x17B560, PlayLayer_destroyPlayer_H, PlayLayer_destroyPlayer);
	Hook(base + 0x9B2A0, GameManager_isColorUnlocked_H, GameManager_isColorUnlocked);
	Hook(base + 0x16C830, PlayLayer_levelComplete_H, PlayLayer_levelComplete);
	Hook(base + 0x170F30, PlayLayer_update_H, PlayLayer_update);
	Hook(base + 0x16AB80, PlayLayer_init_H, PlayLayer_init);
	// Hook(base + 0xEA4C0, LevelBrowserLayer_init_H, LevelBrowserLayer_init);
    // Hook(base + 0x56FA0, EditLevelLayer_init_H, EditLevelLayer_init);

    MH_EnableHook(MH_ALL_HOOKS);
    return 0;
}


/*
This function runs whenever we inject our dll, this is present
in pretty much every mod, and is just winapi boilerplate
*/
BOOL APIENTRY DllMain(HMODULE handle, DWORD reason, LPVOID reserved) {
    if (reason == DLL_PROCESS_ATTACH) {
        auto h = CreateThread(0, 0, thread_func, handle, 0, 0);
        if (h)
            CloseHandle(h);
        else
            return FALSE;
    }
    return TRUE;
}