workspace "InfSysCAD"
	architecture "x64"
	startproject "InfSysCAD"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["glfw"]  = "InfSysCAD/products/glfw/include"
IncludeDir["imgui"] = "InfSysCAD/products/imgui"
IncludeDir["occt"] = "InfSysCAD/products/occt-install/inc"

group "Dependencies"
	include "InfSysCAD/products/glfw"
	include "InfSysCAD/products/imgui"

group ""

project "InfSysCAD"
	location "InfSysCAD"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "infSys_pch.h"
	pchsource "InfSysCAD/src/infSys_pch.cpp" --only for VisualStudio

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	postbuildcommands
    {
  --      {"{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/InfSysCAD"}
    }

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"InfSysCAD/src", 
		"%{prj.name}/products/spdlog/include",	
		"%{IncludeDir.glfw}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.occt}",
	}

	libdirs 
	{
		"InfSysCAD/products/occt-install/win64/vc14/libd"
	}

	links
	{
		"GLFW",
		"ImGui",
		"opengl32.lib",

		-- "TKernel",
		-- "TKOpenGl",
		-- "TKPrim",
		-- "TKService",
		-- "TKSTEP",
		-- "TKTopAlgo",
		-- "TKV3d",
		-- "TKXSBase",	 
		-- "TKMAth",
		-- "TKBRep",
		-- "TKGeomBase",
		-- "TKG3d",
		-- "TKG2d",		
		-- "TKSTEPAttr",
		-- "TKSTEP209",
		-- "TKSTEPBase",
		-- "TKShHealing",
		-- "TKGeomAlgo",
		-- "TKMesh",
		-- "TKHLR",

		"TKCDF.lib",
		"TKDCAF.lib",
		"TKDraw.lib",
		"TKernel.lib",
		"TKFeat.lib",
		"TKFillet.lib",
		"TKG2d.lib",
		"TKG3d.lib",
		"TKGeomAlgo.lib",
		"TKGeomBase.lib",
		"TKHLR.lib",
		"TKIGES.lib",
		--"TKIVtk.lib",
		--"TKIVtkDraw.lib",
		"TKLCAF.lib",
		"TKMath.lib",
		"TKMesh.lib",
		"TKMeshVS.lib",
		"TKOffset.lib",
		"TKOpenGl.lib",
		"TKOpenGlTest.lib",
		"TKPrim.lib",
		"TKQADraw.lib",
		"TKRWMesh.lib",
		"TKService.lib",
		"TKShHealing.lib",
		"TKStd.lib",
		"TKStdL.lib",
		"TKSTEP.lib",
		"TKSTEP209.lib",
		"TKSTEPAttr.lib",
		"TKSTEPBase.lib",
		"TKSTL.lib",
		"TKTObj.lib",
		"TKTObjDRAW.lib",
		"TKTopAlgo.lib",
		"TKTopTest.lib",
		"TKV3d.lib",
		"TKVCAF.lib",
		"TKViewerTest.lib",
		"TKVRML.lib",
		"TKXCAF.lib",
		"TKXDEDRAW.lib",
		"TKXDEIGES.lib",
		"TKXDESTEP.lib",
		"TKXMesh.lib",
		"TKXml.lib",
		"TKXmlL.lib",
		"TKXmlTObj.lib",
		"TKXmlXCAF.lib",
		"TKXSBase.lib",
		"TKXSDRAW.lib",
		"TKBin.lib",
		"TKBinL.lib",
		"TKBinTObj.lib",
		"TKBinXCAF.lib",
		"TKBO.lib",
		"TKBool.lib",
		"TKBRep.lib",
		"TKCAF.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		debugenvs { "PATH=$(SolutionDir)InfSysCAD\\products\\occt-install\\win64\\vc14\\bind;$(SolutionDir)products\\freetype-2.5.5-vc14-64\\bin;%PATH%" }
		libdirs { "InfSysCAD/products/occt-install/win64/vc14/libd" }
		defines "INFSYS_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		debugenvs { "PATH=$(SolutionDir)InfSysCAD\\products\\occt-install\\win64\\vc14\\bin;$(SolutionDir)products\\freetype-2.5.5-vc14-64\\bin;%PATH%" }
		libdirs { "InfSysCAD/products/occt-install/win64/vc14/lib" }
		defines "INFSYS_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		libdirs { "InfSysCAD/products/occt-install/win64/vc14/libd" }
		defines "INFSYS_DIST"
		runtime "Release"
		optimize "on"