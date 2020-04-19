#include "EngineWindow.h"

Engine::Window::EngineWindow::EngineWindow()
{
}

Engine::Window::EngineWindow::~EngineWindow()
{
}

bool Engine::Window::EngineWindow::Init()
{
	return false;
}

UINT Engine::Window::EngineWindow::GetWidth()
{
	return mWidth;
}

UINT Engine::Window::EngineWindow::GetHeight()
{
	return mHeight;
}

const char * Engine::Window::EngineWindow::GetCaption()
{
	return mCaption;
}
