#ifndef __ENGINE_BOX_APP_H__
#define __ENGINE_BOX_APP_H__

#include "EngineManagerDirectX.h"

using std::bind;
using namespace std::placeholders;
using Engine::EngineManagerDirectX;
using Engine::RenderType;

extern EngineManagerDirectX manager;

class EngineBoxApp
{
public:
	EngineBoxApp();
	~EngineBoxApp();
	int Run(HINSTANCE hInstance);
	void Update(float deltaTime);
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	float mTheta = 1.5f * XM_PI;
	float mPhi = 0.25f * XM_PI;
	float mRadius = 5.0f;

	POINT mLastMousePos;
};

#endif // !__ENGINE_BOX_APP_H__

