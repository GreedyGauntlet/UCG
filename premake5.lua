include "./vendor/flora/vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "UCG"
	architecture "x86_64"
	startproject "UCG"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/flora/vendor/premake"
	include "vendor/flora/Flora/vendor/Box2D"
	include "vendor/flora/Flora/vendor/GLFW"
	include "vendor/flora/Flora/vendor/Glad"
	include "vendor/flora/Flora/vendor/imgui"
	include "vendor/flora/Flora/vendor/msdf-atlas-gen"
	include "vendor/flora/Flora/vendor/yaml-cpp"
	include "vendor/flora/Flora/vendor/openAL"
group ""

group "Core"
	include "vendor/flora/Flora"
	include "vendor/flora/Flora-ScriptCore"
group ""

group "Tools"
	include "vendor/flora/Fauna"
	include "vendor/flora/SandboxBuild"
group ""

project "UCG"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/vendor/flora/Flora/vendor/spdlog/include",
		"%{wks.location}/vendor/flora/Flora/src",
		"%{wks.location}/vendor/flora/Flora/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.openAL}"
	}

	links
	{
		"Flora"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
		}

		links
		{
			"%{Library.Shell32}",
		}

	filter "configurations:Debug"
		defines "FL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "FL_DIST"
		runtime "Release"
		optimize "on"
