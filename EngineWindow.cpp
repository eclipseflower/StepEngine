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

string Engine::Window::EngineWindow::GetCaption() const
{
	return mCaption;
}

void Engine::Window::EngineWindow::SetCaption(string caption)
{
	mCaption = caption;
}
