/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef _GLOBAL_RENDERING_H
#define _GLOBAL_RENDERING_H

#include <string>
#include <memory>

#include "System/creg/creg_cond.h"
#include "System/Misc/SpringTime.h"
#include "System/type2.h"

struct SDL_version;
struct SDL_Window;
typedef void* SDL_GLContext;

class CMatrix44f;

/**
 * @brief Globally accessible unsynced, rendering related data
 *
 * Contains globally accessible rendering related data
 * that does not remain synced.
 */
class CGlobalRendering {
	CR_DECLARE_STRUCT(CGlobalRendering)

public:
	CGlobalRendering();
	~CGlobalRendering();

	void PreKill();

	static void InitStatic();
	static void KillStatic();

	/**
	 * @return whether setting the video mode was successful
	 *
	 * Sets SDL video mode options/settings
	 */
	bool CreateWindowAndContext(const char* title, bool hidden);
	SDL_Window* CreateSDLWindow(const int2& winRes, const int2& minRes, const char* title, bool hidden) const;
	SDL_GLContext CreateGLContext(const int2& minCtx, SDL_Window* targetWindow) const;
	SDL_Window* GetWindow(size_t i) { return sdlWindows[i]; }
	SDL_GLContext GetContext(size_t i) { return glContexts[i]; }

	void DestroyWindowAndContext(SDL_Window* window, SDL_GLContext context);
	void KillSDL() const;
	void PostInit();
	void SwapBuffers(bool allowSwapBuffers, bool clearErrors);

	void MakeCurrentContext(bool hidden, bool secondary, bool clear);

	void CheckGLExtensions() const;
	void SetGLSupportFlags();
	void QueryVersionInfo(char (&sdlVersionStr)[64], char (&glVidMemStr)[64]);
	void QueryGLMaxVals();
	void LogVersionInfo(const char* sdlVersionStr, const char* glVidMemStr) const;
	void LogDisplayMode(SDL_Window* window) const;

	void SetWindowTitle(const std::string& title);
	// Notify on Fullscreen/WindowBorderless change
	void ConfigNotify(const std::string& key, const std::string& value);

	bool SetWindowInputGrabbing(bool enable);
	bool ToggleWindowInputGrabbing();

	void SetFullScreen(bool cliWindowed, bool cliFullScreen);
	void SetDualScreenParams();
	void UpdateViewPortGeometry();
	void UpdatePixelGeometry();
	void ReadWindowPosAndSize();
	void SaveWindowPosAndSize();
	void UpdateGLConfigs();
	void UpdateGLGeometry();
	void UpdateScreenMatrices();

	int2 GetScreenCenter() const { return {viewPosX + (viewSizeX >> 1), viewPosY + (viewSizeY >> 1)}; }
	int2 GetMaxWinRes() const;
	int2 GetCfgWinRes(bool fullScrn) const;

	bool CheckGLMultiSampling() const;
	bool CheckGLContextVersion(const int2& minCtx) const;
	bool ToggleGLDebugOutput(unsigned int msgSrceIdx, unsigned int msgTypeIdx, unsigned int msgSevrIdx);
	void InitGLState();

public:
	/**
	 * @brief time offset
	 *
	 * Time in number of frames since last update
	 * (for interpolation)
	 */
	float timeOffset;

	/**
	 * @brief last frame time
	 *
	 * How long the last draw cycle took in real time (MILLIseconds)
	 */
	float lastFrameTime;

	/// the starting time in tick for last draw frame
	spring_time lastFrameStart;

	/// 0.001f * gu->simFPS, used for rendering
	float weightedSpeedFactor;

	/// the draw frame number (never 0)
	unsigned int drawFrame;

	/// Frames Per Second
	float FPS;


	/// the screen size in pixels
	int screenSizeX;
	int screenSizeY;

	/// the window position relative to the screen's bottom-left corner
	int winPosX;
	int winPosY;

	/// the window size in pixels
	int winSizeX;
	int winSizeY;

	/// the viewport position relative to the window's bottom-left corner
	int viewPosX;
	int viewPosY;

	/// the viewport size in pixels
	int viewSizeX;
	int viewSizeY;

	/// screen {View,Proj} matrices for rendering in pixel coordinates
	std::unique_ptr<CMatrix44f> screenViewMatrix;
	std::unique_ptr<CMatrix44f> screenProjMatrix;

	/// size of one pixel in viewport coordinates, i.e. 1/viewSizeX and 1/viewSizeY
	float pixelX;
	float pixelY;

	float minViewRange;
	float maxViewRange;
	/**
	 * @brief aspect ratio
	 *
	 * (float)viewSizeX / (float)viewSizeY
	 */
	float aspectRatio;

	int forceDisablePersistentMapping;
	int forceDisableShaders;
	int forceCoreContext;
	int forceSwapBuffers;

	/**
	 * @brief MSAA
	 *
	 * Level of multisample anti-aliasing
	 */
	int msaaLevel;

	/**
	 * @brief maxTextureSize
	 *
	 * maximum 2D texture size
	 */
	int maxTextureSize;

	float maxTexAnisoLvl;


	bool drawSky;
	bool drawWater;
	bool drawGround;
	bool drawMapMarks;

	/**
	 * @brief draw fog
	 *
	 * Whether fog (of war) is drawn or not
	 */
	bool drawFog;

	/**
	 * @brief draw debug
	 *
	 * Whether debugging info is drawn
	 */
	bool drawDebug;
	bool drawDebugTraceRay;
	bool drawDebugCubeMap;

	bool glDebug;
	bool glDebugErrors;

	/**
	 * Does the user want team colored nanospray?
	 */
	bool teamNanospray;


	/**
	 * @brief active video
	 *
	 * Whether the graphics need to be drawn
	 */
	bool active;

	bool grabInput; //what it says

	/**
	 * @brief compressTextures
	 *
	 * If set, many (not all) textures will compressed on run-time.
	*/
	bool compressTextures;

	/**
	 * @brief GPU driver's vendor
	 *
	 * These can be used to enable workarounds for bugs in their drivers.
	 * Note, you should always give the user the possiblity to override such workarounds via config-tags.
	 */
	bool haveAMD;
	bool haveMesa;
	bool haveIntel;
	bool haveNvidia;


	/**
	 * @brief collection of some ATI bugfixes
	 *
	 * enables some ATI bugfixes
	 */
	bool amdHacks;

	/**
	* @brief whether the GPU supports persistent buffer mapping
	*
	* ARB_buffer_storage or OpenGL 4.4
	*/
	bool supportPersistentMapping;

	/**
	 * @brief if the GPU (drivers) support NonPowerOfTwoTextures
	 *
	 * Especially some ATI cards report that they support NPOTs, but don't (or just very limited).
	 */
	bool supportNonPowerOfTwoTex;
	bool supportTextureQueryLOD;

	bool supportMSAAFrameBuffer;

	int supportDepthBufferBestBits;
	bool supportDepthBufferBits[4];

	bool supportRestartPrimitive;
	bool supportClipSpaceControl;
	bool supportSeamlessCubeMaps;
	bool supportFragDepthLayout;

	/**
	 * Shader capabilities
	 */
	int glslMaxVaryings;
	int glslMaxAttributes;
	int glslMaxDrawBuffers;
	int glslMaxRecommendedIndices;
	int glslMaxRecommendedVertices;
	int glslMaxUniformBufferBindings;
	int glslMaxUniformBufferSize; ///< in bytes
	int glslMaxStorageBufferBindings;
	int glslMaxStorageBufferSize; ///< in bytes

	/**
	 * @brief dual screen mode
	 * In dual screen mode, the screen is split up between a game screen and a minimap screen.
	 * In this case viewSizeX is half of the actual GL viewport width,
	 */
	bool dualScreenMode;

	/**
	 * @brief dual screen minimap on left
	 * In dual screen mode, allow the minimap to either be shown on the left or the right display.
	 * Minimap on the right is the default.
	 */
	bool dualScreenMiniMapOnLeft;

	/**
	 * @brief full-screen or windowed rendering
	 */
	bool fullScreen;
	bool borderless;

public:
	// [0] := primary, [1] := secondary (hidden)
	SDL_Window* sdlWindows[2];
	SDL_GLContext glContexts[2];
public:
	//helper function
	static int DepthBitsToFormat(int bits);
public:
	/**
	* @brief maximum texture unit number
	*/
	static constexpr int MAX_TEXTURE_UNITS = 32;
	/**
	* @brief max view range in elmos
	*/
	static constexpr float MAX_VIEW_RANGE = 65536.0f;

	/**
	* @brief near z-plane distance in elmos
	*/
	static constexpr float MIN_ZNEAR_DIST = 0.5f;


	/// magic constant to reduce overblending on SMF maps
	/// (scales the MapInfo::light_t::ground*Color values;
	/// roughly equal to 210.0f / 255.0f)
	static constexpr float SMF_INTENSITY_MULT = (210.0f / 256.0f) + (1.0f / 256.0f) - (1.0f / 2048.0f) - (1.0f / 4096.0f);


	static constexpr int MIN_WIN_SIZE_X = 400;
	static constexpr int MIN_WIN_SIZE_Y = 300;
};

extern CGlobalRendering* globalRendering;

#endif /* _GLOBAL_RENDERING_H */

