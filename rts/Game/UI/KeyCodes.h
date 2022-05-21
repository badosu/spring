/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef KEYCODES_H
#define KEYCODES_H

#include "IKeys.h"

class CKeyCodes : public IKeys {
public:

	bool IsModifier(int code);

	void Reset();
	void PrintNameToCode() const;
	void PrintCodeToName() const;
};

extern CKeyCodes keyCodes;

#endif /* KEYCODES_H */
