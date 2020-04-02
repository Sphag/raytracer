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
		symbols "On"
	
	-- We now only set settings for Release
	filter { "configurations:Release" }
		-- Release should be optimized
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
    
    files
	{
		"raytracer/%{prj.name}/include/**.h",
		"raytracer/%{prj.name}/src/**.cpp",
		"raytracer/vendor/glm/glm/**.hpp",
		"raytracer/vendor/glm/glm/**.inl",
		"raytracer/vendor/stb_image/**.h",
		"raytracer/vendor/stb_image/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	links
	{
		"utils"
	}

	includedirs
	{
		"raytracer/%{prj.name}/include",
		"raytracer/utils/include/",
		"raytracer/vendor/glm",
		"raytracer/vendor/stb_image/",
	}

	project "utils"

    location "raytracer/utils"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
    
    files
	{
		"raytracer/utils/include/utils/**.h",
		"raytracer/utils/src/**.cpp",
		"raytracer/vendor/stb_image/**.h",
		"raytracer/vendor/stb_image/**.cpp",
		"raytracer/vendor/glm/glm/**.hpp",
		"raytracer/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"raytracer/%{prj.name}/include/utils",
		"raytracer/vendor/stb_image/",
		"raytracer/vendor/glm",
	}
