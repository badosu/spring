/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef RML_UI_SYSTEM_H
#define RML_UI_SYSTEM_H

#include <RmlUi/Core/SystemInterface.h>

class CRmlUI : public Rml::SystemInterface {
public:
	CRmlUI();
	~CRmlUI();

	// Get the number of seconds elapsed since the start of the application.
	virtual double GetElapsedTime();
	// Translate the input string into the translated string.
	virtual int TranslateString(Rml::String& translated, const Rml::String& input) { return 0; };
	// Joins the path of an RML or RCSS file with the path of a resource specified within the file.
	virtual void JoinPath(std::string& translated_path, const Rml::String& document_path, const Rml::String& path) {}
	// Log the specified message.
	virtual bool LogMessage(Rml::Log::Type type, const Rml::String& message) { return false; }
	// Set the mouse cursor.
	virtual void SetMouseCursor(const Rml::String& cursor_name) {}
	// Set clipboard text.
	virtual void SetClipboardText(const Rml::String& text) {}
	// Get clipboard text.
	virtual void GetClipboardText(Rml::String& text) {}
	// Activate keyboard (for touchscreen devices).
	virtual void ActivateKeyboard(Rml::Vector2f caret_position, float line_height) {}
	// Deactivate keyboard (for touchscreen devices).
	virtual void DeactivateKeyboard() {}

};

#endif /* RML_UI_H */
