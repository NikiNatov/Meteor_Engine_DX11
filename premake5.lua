workspace "Meteor"
	architecture "x64"
	startproject "Meteor"

	configurations 
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg} - %{cfg.system}"

	IncludeDirs = {}
	IncludeDirs["spdlog"]		= "Meteor/vendor/spdlog/include"
	IncludeDirs["stb_image"]	= "Meteor/vendor/stb_image"
	IncludeDirs["entt"]			= "Meteor/vendor/entt/include"
	IncludeDirs["ImGui"]		= "Meteor/vendor/imgui"
	IncludeDirs["glm"]			= "Meteor/vendor/glm"

group "Dependencies"
	include "Meteor/vendor/ImGui"
project "Meteor"
	location "Meteor"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	systemversion "latest"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "%{prj.name}/source/pch.cpp"

	files
	{
		"%{prj.name}/source/**.cpp",
		"%{prj.name}/source/**.h",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/source",
		"%{IncludeDirs.spdlog}",
		"%{IncludeDirs.glm}",
		"%{IncludeDirs.ImGui}",
		"%{IncludeDirs.stb_image}",
		"%{IncludeDirs.entt}"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	links
	{
		"ImGui"
	}

	filter "configurations:Debug"
		defines "METEOR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "METEOR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "system:Windows"
		defines "METEOR_PLATFORM_WINDOWS"
		links "d3d11.lib"
		links "D3DCompiler.lib"

	characterset("ASCII")