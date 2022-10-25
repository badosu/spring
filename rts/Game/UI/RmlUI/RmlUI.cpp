#include "RmlUI.h"
#include "System/Misc/SpringTime.h"

CRmlUI::CRmlUI()
{
}

CRmlUI::~CRmlUI()
{
}

double CRmlUI::GetElapsedTime()
{
	return spring_gettime().toSecs<double>();
}
