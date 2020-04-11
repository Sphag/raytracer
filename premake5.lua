workspace "raytracer"
	-- We set the location of the files Premake will generate
	location "generated"
	
	-- We indicate that all the projects are C++ only
	language "C++"
	
	-- We will compile for x86_64. You can change this to x86 for 32 bit builds.
	architecture "x86_64"
	
	-- Configurations are often used to store some compiler / linker settings together.
    -- The Debug configuration will be used by us while debugging.
    -- The optimized Release configuration will be used when shipping the app.
	configurations { "Debug", "Release" }
	
	-- We use filters to set options, a new feature of Premake5.
	
	-- We now only set settings for the Debug configuration
	filter { "configurations:Debug" }
		-- We want debug symbols in our debug config
		defines "RT_DEBUG"
		symbols "On"
	
	-- We now only set settings for Release
	filter { "configurations:Release" }
		-- Release should be optimized
		defines "RT_RELEASE"
		optimize "On"
	
	-- Reset the filter for other settings
	filter { }
	
	-- Here we use some "tokens" (the things between %{ ... }). They will be replaced by Premake
	-- automatically when configuring the projects.
	-- * %{prj.name} will be replaced by "ExampleLib" / "App" / "UnitTests"
	--  * %{cfg.longname} will be replaced by "Debug" or "Release" depending on the configuration
    -- The path is relative to *this* folder
    targetdir ("build/bin/%{prj.name}/%{cfg.longname}")
	objdir ("build/obj/%{prj.name}/%{cfg.longname}")
    
    project "raytracer"

    location "raytracer/raytracer"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"

	pchheader "rtpch.h"
	pchsource "raytracer/raytracer/src/rtpch.cpp"
	  
    files
	{
		"raytracer/%{prj.name}/include/**.h",
		"raytracer/%{prj.name}/include/core/**.h",
		"raytracer/%{prj.name}/include/materials/**.h",
		"raytracer/%{prj.name}/include/objects/**.h",
		"raytracer/%{prj.name}/include/ray_tracer/**.h",		
		"raytracer/%{prj.name}/src/**.cpp",
		"raytracer/%{prj.name}/src/core/**.cpp",
		"raytracer/%{prj.name}/src/materials/**.cpp",
		"raytracer/%{prj.name}/src/objects/**.cpp",
		"raytracer/%{prj.name}/src/ray_tracer/**.cpp",
		"raytracer/vendor/glm/glm/**.hpp",
		"raytracer/vendor/glm/glm/**.inl",
		"raytracer/vendor/stb_image/**.h",
		"raytracer/vendor/stb_image/**.cpp"
	}

	links
	{
		"cds"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"UTILS_USE_DEFAULT_PIXEL_TYPES"
	}

	includedirs
	{
		"raytracer/%{prj.name}/include",
		"raytracer/vendor/glm",
		"raytracer/vendor/stb_image/",
		"raytracer/vendor/libcds/",
	}

	
	filter {"files:raytracer/vendor/stb_image/**.cpp"}
		flags {"NoPCH"}
	filter { }

	
	project "cds"
	location "raytracer/vendor/libcds/"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"

	filter { "configurations:Debug" }
		postbuildcommands { "copy ..\\..\\..\\build\\bin\\cds\\Debug\\cds.dll ..\\..\\..\\build\\bin\\raytracer\\Debug\\" }
	filter { }
	filter { "configurations:Release" }
		postbuildcommands { "copy ..\\..\\..\\build\\bin\\cds\\Release\\cds.dll ..\\..\\..\\build\\bin\\raytracer\\Release\\" }
	filter { }
	filter { "configurations:Debug" }
		defines
		{
			"_ENABLE_ATOMIC_ALIGNMENT_FIX",
			"WIN32",
			"_DEBUG",
			"_CONSOLE",
			"_WIN32_WINNT=0x0501",
			"CDS_BUILD_LIB"
		}
	filter { }
	filter { "configurations:Release" }
		defines
		{
			"_ENABLE_ATOMIC_ALIGNMENT_FIX",
			"WIN32",
			"NDEBUG",
			"_CONSOLE",
			"_WIN32_WINNT=0x0501",
			"CDS_BUILD_LIB",
		}
	filter { }
	includedirs
	{
		"raytracer/vendor/libcds/",
	}

	files
	{
		"raytracer/vendor/libcds/cds/**",
		"raytracer/vendor/libcds/src/**.cpp",
	}
