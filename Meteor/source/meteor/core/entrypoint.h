#pragma once

#if defined(METEOR_PLATFORM_WINDOWS)
#include "platform\windows\win.h"
#endif

#include "application.h"
#include "logger.h"

extern meteor::Application* meteor::CreateApplication();

#if defined(METEOR_PLATFORM_WINDOWS)

#if defined(METEOR_DEBUG)
int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	meteor::Logger::Initialize();

	auto app = meteor::CreateApplication();
	app->Run();
	delete app;
}
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	meteor::Logger::Initialize();

	auto app = meteor::CreateApplication();
	app->Run();
	delete app;
}
#endif // METEOR_DEBUG

#else
#error Meteor only supports Windows!
#endif // METEOR_PLATFORM_WINDOWS