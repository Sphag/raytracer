workspace "raytracer"
	location "generated"
	
	language "C++"
	
	architecture "x86_64"

	configurations { "Debug", "Release" }

	filter { "configurations:Debug" }
		defines "RT_DEBUG"
		symbols "On"
	
	filter { "configurations:Release" }
		-- Release should be optimized
		defines "RT_RELEASE"
		optimize "On"
	
	filter { }

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
		"raytracer/vendor/stb_image/**.cpp",
		"raytracer/vendor/tinyobjloader/tiny_obj_loader.h",
		"raytracer/vendor/tinyobjloader/tiny_obj_loader.cc",
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
		"raytracer/vendor/tinyobjloader/",
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
