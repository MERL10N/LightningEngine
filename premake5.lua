workspace "LightningEngine"
    configurations { "Debug", "Release" }
    filter "system:macosx"
        architecture "ARM64"
    filter {}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Lightning Core (static library)
project "LightningCore"
    location "LightningCore"
    kind "StaticLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    { 
        "LightningCore/**.h", 
        "LightningCore/**.cpp",
        "LightningCore/**.mm",
        "ThirdParty/imgui/backends/imgui_impl_glfw.cpp",
        "ThirdParty/imgui/backends/imgui_impl_metal.mm",
        "ThirdParty/imgui/backends/imgui_impl_glfw.h",
        "ThirdParty/imgui/backends/imgui_impl_metal.h",
        "ThirdParty/imgui/imgui.cpp",
        "ThirdParty/imgui/imgui_draw.cpp",
        "ThirdParty/imgui/imgui_tables.cpp",
        "ThirdParty/imgui/imgui_widgets.cpp",
        "ThirdParty/imgui/imgui_demo.cpp",
        "ThirdParty/entt/single_include"
    }

    includedirs 
    { 
        "LightningCore",
        "ThirdParty",
        "ThirdParty/imgui",
        "ThirdParty/glfw/include",
        "ThirdParty/glm",
        "ThirdParty/stb",
        "ThirdParty/entt/single_include",
        "ThirdParty/metal-cpp",
    }

    filter "system:macosx"
        cppdialect "C++23"
        staticruntime "On"

     
        links   
        { 
            "Metal.framework", 
            "MetalKit.framework", 
            "QuartzCore.framework",
            "Cocoa.framework",
            "AppKit.framework",
            "GameController.framework",
            "CoreGraphics.framework",
            "CoreAnimation.framework",
            "IOKit.framework",
            "CoreVideo.framework",
            "Foundation.framework"
        }

        buildoptions { "-std=c++23", "-stdlib=libc++", "-fobjc-arc"}
        linkoptions  { "-stdlib=libc++" }
    filter {}

        externalincludedirs { "ThirdParty/metal-cpp", "ThirdParty/metal-cpp-extensions", "ThirdParty/stb", "ThirdParty/imgui", "ThirdParty/spdlog/include", "ThirdParty/glfw/include"}
    filter {}

-- Lightning Game (app)
project "LightningGame"
    location "LightningGame"
    kind "WindowedApp"   
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "LightningGame/Source/**.h", 
        "LightningGame/Source/**.cpp",
        "LightningGame/Assets/**",
        "LightningGame/Assets/Shaders/**.metal"
    }

    includedirs { "ThirdParty", "LightningGame/Source", "LightningCore" }
    libdirs { "" }
    links { "LightningCore" }

    filter "system:macosx"
        cppdialect "C++23"
        staticruntime "On"

        filter { "system:macosx", "files:LightningGame/Assets/**", "LightningGame/Assets/Shaders/**" }
        buildaction "Resource"
        filter {}

        libdirs { "ThirdParty/glfw/lib-universal" }

        links 
        { 
            "LightningCore",
            "Metal.framework",
            "MetalKit.framework",
            "QuartzCore.framework",
            "Cocoa.framework",
            "AppKit.framework",
            "GameController.framework",
            "CoreGraphics.framework",
            "IOKit.framework",
            "CoreVideo.framework",
            "Foundation.framework",
        }

        linkoptions { "-FThirdParty/glfw/lib-universal", "-lglfw3" }

        xcodebuildsettings
        {
            ["GENERATE_INFOPLIST_FILE"] = "YES",
            ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.yourcompany.LightningGame",
            ["MACOSX_DEPLOYMENT_TARGET"] = "26.0",
            ["MTL_HEADER_SEARCH_PATHS"] = "%{wks.location}/../../LightningCore",
            ["OTHER_METALCOMPILER_FLAGS"] = "-I\"$(SRCROOT)/../../LightningCore\""
        }

        postbuildcommands
        {
            "cp -R ../LightningEditor/Assets/ %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Assets",
        }

    filter {}

    externalincludedirs {"ThirdParty/metal-cpp", "ThirdParty/metal-cpp-extensions", "LightningCore/", "ThirdParty/stb", "ThirdParty/glfw/include"}
    libdirs { "ThirdParty/glfw/lib-universal" }
    links { "glfw3" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter {}

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"
    filter {}

    -- Lightning Editor (app)
project "LightningEditor"
    location "LightningEditor"
    kind "WindowedApp"   
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files 
    {
        "LightningEditor/Source/main.cpp", 
        "LightningEditor/Assets/**",
    }

    includedirs { "LightningCore", "ThirdParty", "ThirdParty/imgui", "ThirdParty/glfw/include", "ThirdParty/metal-cpp" }

    filter "system:macosx"
        cppdialect "C++23"
        staticruntime "On"
        
        files
        { 
            "LightningEditor/Source/**.cpp",
            "LightningEditor/Source/**.h",  
            "LightningEditor/Assets/Shaders/**.metal",
            "LightningEditor/Assets/**",
        }

        filter { "system:macosx", "files:LightningEditor/Assets/**", "LightningEditor/Assets/Shaders/**" }
        buildaction "Resource"
        filter {}

        libdirs { "ThirdParty/glfw/lib-universal" }

        links 
        { 
            "LightningCore",
            "Metal.framework",
            "MetalKit.framework",
            "QuartzCore.framework",
            "Cocoa.framework",
            "AppKit.framework",
            "GameController.framework",
            "CoreGraphics.framework",
            "IOKit.framework",
            "CoreVideo.framework",
            "Foundation.framework",
        }

        linkoptions { "-FThirdParty/glfw/lib-universal", "-lglfw3" }

        xcodebuildsettings
        {
            ["GENERATE_INFOPLIST_FILE"] = "YES",
            ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.yourcompany.LightningEditor",
            ["MACOSX_DEPLOYMENT_TARGET"] = "26.0",
            ["MTL_HEADER_SEARCH_PATHS"] = "%{wks.location}/../../LightningCore",
            ["OTHER_METALCOMPILER_FLAGS"] = "-I\"$(SRCROOT)/../../LightningCore\""
        }

        buildoptions { "-std=c++23", "-stdlib=libc++" }
        linkoptions  { "-stdlib=libc++" }

        postbuildcommands
        {
            "cp -R ../LightningEditor/Assets/ %{cfg.buildtarget.directory}/%{cfg.buildtarget.name}/Contents/Resources/Assets",
        }
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter {}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"   
