/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */


#include <SDL_scancode.h>
#include <SDL_keyboard.h>

#include "ScanCodes.h"
#include "System/Log/ILog.h"
#include "System/StringUtil.h"

CScanCodes scanCodes;

bool CScanCodes::IsModifier(int code)
{
	switch (code) {
		case SDL_SCANCODE_LALT:
		case SDL_SCANCODE_LCTRL:
		case SDL_SCANCODE_LGUI:
		case SDL_SCANCODE_LSHIFT:
		case SDL_SCANCODE_RALT:
		case SDL_SCANCODE_RCTRL:
		case SDL_SCANCODE_RGUI:
		case SDL_SCANCODE_RSHIFT:
			return true;
	}
	return false;
}

void CScanCodes::Reset()
{
	nameToCode.clear();
	nameToCode.reserve(64);
	codeToName.clear();
	codeToName.reserve(64);

	printableCodes.clear();
	printableCodes.reserve(64);

	AddPair("backspace", SDL_SCANCODE_BACKSPACE);
	AddPair("tab",       SDL_SCANCODE_TAB);
	AddPair("clear",     SDL_SCANCODE_CLEAR);
	AddPair("enter",     SDL_SCANCODE_RETURN); //FIXME
	AddPair("return",    SDL_SCANCODE_RETURN);
	AddPair("pause",     SDL_SCANCODE_PAUSE);
	AddPair("esc",       SDL_SCANCODE_ESCAPE);
	AddPair("escape",    SDL_SCANCODE_ESCAPE);
	AddPair("space",     SDL_SCANCODE_SPACE, true);
	AddPair("delete",    SDL_SCANCODE_DELETE);

	// ASCII mapped keysyms
	for (char i = ' '; i <= 'z'; ++i) {
		if (isupper(i))
			continue;

		AddPair("sc_" + std::string(1, i), SDL_GetScancodeFromName(&i), true);
	}

	AddPair("sc_`", SDL_SCANCODE_GRAVE, true);
	AddPair("backquote", SDL_SCANCODE_GRAVE, true);

	// Numeric keypad
	AddPair("sc_numpad0", SDL_SCANCODE_KP_0, true);
	AddPair("sc_numpad1", SDL_SCANCODE_KP_1, true);
	AddPair("sc_numpad2", SDL_SCANCODE_KP_2, true);
	AddPair("sc_numpad3", SDL_SCANCODE_KP_3, true);
	AddPair("sc_numpad4", SDL_SCANCODE_KP_4, true);
	AddPair("sc_numpad5", SDL_SCANCODE_KP_5, true);
	AddPair("sc_numpad6", SDL_SCANCODE_KP_6, true);
	AddPair("sc_numpad7", SDL_SCANCODE_KP_7, true);
	AddPair("sc_numpad8", SDL_SCANCODE_KP_8, true);
	AddPair("sc_numpad9", SDL_SCANCODE_KP_9, true);
	AddPair("sc_numpad.", SDL_SCANCODE_KP_PERIOD, true);
	AddPair("sc_numpad/", SDL_SCANCODE_KP_DIVIDE, true);
	AddPair("sc_numpad*", SDL_SCANCODE_KP_MULTIPLY, true);
	AddPair("sc_numpad-", SDL_SCANCODE_KP_MINUS, true);
	AddPair("sc_numpad+", SDL_SCANCODE_KP_PLUS, true);
	AddPair("sc_numpad=", SDL_SCANCODE_KP_EQUALS, true);
	AddPair("sc_numpad_enter", SDL_SCANCODE_KP_ENTER);

	// Arrows + Home/End pad
	AddPair("sc_up",       SDL_SCANCODE_UP);
	AddPair("sc_down",     SDL_SCANCODE_DOWN);
	AddPair("sc_right",    SDL_SCANCODE_RIGHT);
	AddPair("sc_left",     SDL_SCANCODE_LEFT);
	AddPair("sc_insert",   SDL_SCANCODE_INSERT);
	AddPair("sc_home",     SDL_SCANCODE_HOME);
	AddPair("sc_end",      SDL_SCANCODE_END);
	AddPair("sc_pageup",   SDL_SCANCODE_PAGEUP);
	AddPair("sc_pagedown", SDL_SCANCODE_PAGEDOWN);

	// Function keys
	AddPair("sc_f1",  SDL_SCANCODE_F1);
	AddPair("sc_f2",  SDL_SCANCODE_F2);
	AddPair("sc_f3",  SDL_SCANCODE_F3);
	AddPair("sc_f4",  SDL_SCANCODE_F4);
	AddPair("sc_f5",  SDL_SCANCODE_F5);
	AddPair("sc_f6",  SDL_SCANCODE_F6);
	AddPair("sc_f7",  SDL_SCANCODE_F7);
	AddPair("sc_f8",  SDL_SCANCODE_F8);
	AddPair("sc_f9",  SDL_SCANCODE_F9);
	AddPair("sc_f10", SDL_SCANCODE_F10);
	AddPair("sc_f11", SDL_SCANCODE_F11);
	AddPair("sc_f12", SDL_SCANCODE_F12);
	AddPair("sc_f13", SDL_SCANCODE_F13);
	AddPair("sc_f14", SDL_SCANCODE_F14);
	AddPair("sc_f15", SDL_SCANCODE_F15);

	// Key state modifier keys
	//AddPair("numlock", SDL_SCANCODE_NUMLOCK);
	//AddPair("capslock", SDL_SCANCODE_CAPSLOCK);
	//AddPair("scrollock", SDL_SCANCODE_SCROLLOCK);
	AddPair("sc_shift", SDL_SCANCODE_LSHIFT);
	AddPair("sc_ctrl",  SDL_SCANCODE_LCTRL);
	AddPair("sc_alt",   SDL_SCANCODE_LALT);
	AddPair("sc_meta",  SDL_SCANCODE_LGUI);
	// these can not be used correctly anyway (without special support in other parts of Spring code...)
	//AddPair("super", SDL_SCANCODE_LSUPER);    // Left "Windows" key
	//AddPair("mode", SDL_SCANCODE_MODE);       // "Alt Gr" key
	//AddPair("compose", SDL_SCANCODE_COMPOSE); // Multi-key compose key

	// Miscellaneous function keys
	AddPair("sc_help", SDL_SCANCODE_HELP);
	AddPair("sc_printscreen", SDL_SCANCODE_PRINTSCREEN);
	AddPair("sc_print", SDL_SCANCODE_PRINTSCREEN);
	//AddPair("sysreq", SDL_SCANCODE_SYSREQ);
	//AddPair("break", SDL_SCANCODE_BREAK);
	//AddPair("menu", SDL_SCANCODE_MENU);
	//AddPair("power", SDL_SCANCODE_POWER);     // Power Macintosh power key
	//AddPair("euro", SDL_SCANCODE_EURO);       // Some european keyboards
	//AddPair("undo", SDL_SCANCODE_UNDO);       // Atari keyboard has Undo

	std::sort(nameToCode.begin(), nameToCode.end(), [](const NameCodePair& a, const NameCodePair& b) { return (a.first < b.first); });
	std::sort(codeToName.begin(), codeToName.end(), [](const CodeNamePair& a, const CodeNamePair& b) { return (a.first < b.first); });
	std::sort(printableCodes.begin(), printableCodes.end());

	nameToCode.erase(std::unique(nameToCode.begin(), nameToCode.end(), [](const NameCodePair& a, const NameCodePair& b) { return (a.first == b.first); }), nameToCode.end());
	codeToName.erase(std::unique(codeToName.begin(), codeToName.end(), [](const CodeNamePair& a, const CodeNamePair& b) { return (a.first == b.first); }), codeToName.end());
	printableCodes.erase(std::unique(printableCodes.begin(), printableCodes.end()), printableCodes.end());

	// remember our defaults
	defaultNameToCode.clear();
	defaultNameToCode.resize(nameToCode.size());
	defaultCodeToName.clear();
	defaultCodeToName.resize(codeToName.size());

	std::copy(nameToCode.begin(), nameToCode.end(), defaultNameToCode.begin());
	std::copy(codeToName.begin(), codeToName.end(), defaultCodeToName.begin());
}

void CScanCodes::PrintNameToCode() const
{
	for (const auto& p: nameToCode) {
		LOG("SCANNAME: %13s = 0x%03X", p.first.c_str(), p.second);
	}
}

void CScanCodes::PrintCodeToName() const
{
	for (const auto& p: codeToName) {
		LOG("SCANCODE: 0x%03X = '%s'", p.first, p.second.c_str());
	}
}

extern CScanCodes scanCodes;
