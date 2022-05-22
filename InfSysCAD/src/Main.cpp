#pragma once
#include "infSys_pch.h"
#include "CAD/Core/Application.h"

int main(int argc, char** argv)
{
	InfSysCAD::Log::Init();

	INFSYS_PROFILE_BEGIN_SESSION("Startup", "InfSysCAD_Profile-Startup.json");
	auto app = new InfSysCAD::Application();
	INFSYS_PROFILE_END_SESSION();

	INFSYS_PROFILE_BEGIN_SESSION("Runtime", "InfSysCAD_Runtime.json");
	app->Run();
	INFSYS_PROFILE_END_SESSION();

	INFSYS_PROFILE_BEGIN_SESSION("Shutdown", "InfSysCAD_Shutdown.json");
	delete app;
	INFSYS_PROFILE_END_SESSION();

	return 0;
}