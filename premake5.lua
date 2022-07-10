workspace "Resurge"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/"

project "Resurge"
	location "Resurge"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "%{prj.name}")
	objdir("bin-int/" .. outputdir .. "%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Resurge/src",
		"Resurge/vendor/spdlog/include",
		"Resurge/vendor/GLFW/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines "RS_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "RS_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "RS_DIST"
		optimize "On"
