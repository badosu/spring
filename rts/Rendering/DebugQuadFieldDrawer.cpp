/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "DebugQuadFieldDrawer.h"

#include "Game/Camera.h"
#include "Game/GlobalUnsynced.h"
#include "Map/ReadMap.h"
#include "Map/Ground.h"
#include "Rendering/GL/glExtra.h"
#include "Rendering/GL/myGL.h"
#include "Rendering/GlobalRendering.h"
#include "Rendering/Models/3DModel.h"
#include "Rendering/Units/UnitDrawer.h"
#include "Sim/Features/Feature.h"
#include "Sim/Misc/CollisionVolume.h"
#include "Sim/Misc/GlobalConstants.h"
#include "Sim/Misc/QuadField.h"
#include "Sim/Units/Unit.h"
#include "Sim/Units/UnitTypes/Factory.h"
#include "Sim/Weapons/PlasmaRepulser.h"
#include "Sim/Weapons/Weapon.h"
#include "System/UnorderedSet.hpp"

static constexpr float4 DEFAULT_VOLUME_COLOR =
float4(0.45f, 0.0f, 0.45f, 0.35f);
static unsigned int volumeDisplayListIDs[] = {0};
static const float squareSize = CQuadField::BASE_QUAD_SIZE / SQUARE_SIZE;

class CDebugQuadFieldDrawer : public CReadMap::IQuadDrawer {
	public:
		void ResetState() {}
		void DrawQuad(int x, int y) {
			glColorf4(DEFAULT_VOLUME_COLOR);
			glEnable(GL_DEPTH_TEST);

			glPushMatrix();
			const float qx = x * CQuadField::BASE_QUAD_SIZE;
			const float qz = y * CQuadField::BASE_QUAD_SIZE;
			const float3 mid = float3(qx, CGround::GetHeightReal(qx, qz, false), qz) + 50;
			glTranslatef3(mid);
			glScalef(CQuadField::BASE_QUAD_SIZE, CQuadField::BASE_QUAD_SIZE, CQuadField::BASE_QUAD_SIZE);
			glWireCube(&volumeDisplayListIDs[0]);
			glPopMatrix();
		}
};

namespace DebugQuadFieldDrawer {
	bool enable = true;

	void Draw() {
		if (!enable)
			return;

		glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		glDisable(GL_CULL_FACE);
		glDisable(GL_TEXTURE_2D);
		// glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_FOG);
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_CLIP_PLANE1);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glLineWidth(2.0f);
		glDepthMask(GL_TRUE);

		static CDebugQuadFieldDrawer drawer;

		drawer.ResetState();
		readMap->GridVisibility(nullptr, &drawer, 1e9, squareSize);

		glLineWidth(1.0f);
		glPopAttrib();
	}

} // namespace DebugQuadFieldDrawer
