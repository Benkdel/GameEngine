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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glad/src/glad.c",
		"%{prj.name}/vendor/include/STB/stb_image.cpp"
	}

	includedirs
	{
		"Resurge/src",
		"Resurge/vendor/spdlog/include",
		"Resurge/vendor/include",
		"Resurge/vendor/include/GLAD",
		"Resurge/vendor/GLM"
	}

	libdirs
	{
		"Resurge/vendor/libs/"
	}

	links
	{
		"/GLFW/glfw3.lib",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "off"
		runtime "Release"
		systemversion "latest"

	filter "configurations:Debug"
		defines 
		{
			"AB_ENABLE_ASSERTS",
			"AB_DEBUG"
		}
		symbols "On"
		
	filter "configurations:Release"
		defines "AB_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "AB_DIST"
		optimize "On"
