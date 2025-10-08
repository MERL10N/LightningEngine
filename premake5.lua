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

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter {}

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"
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
        "LightningGame/Shaders/Shader.metal",
    }

    includedirs { "LightningCore", "ThirdParty", "ThirdParty/imgui", "ThirdParty/glfw/include", "ThirdParty/metal-cpp", "ThirdParty/metal-cpp-extensions" }

    filter "system:macosx"
        cppdialect "C++23"
        staticruntime "On"
        
        files
        { 
            "LightningGame/Source/**.cpp",
            "LightningGame/Source/**.h",
            "LightningGame/Shaders/Shader.metal",
            "LightningGame/Assets/**",
        }

        filter { "system:macosx", "files:LightningGame/Assets/**" }
        buildaction "Resource"
        filter { "system:macosx", "files:LightningGame/Shaders/**" }
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
            ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.yourcompany.LightningGame"
        }

        buildoptions { "-std=c++23", "-stdlib=libc++" }
        linkoptions  { "-stdlib=libc++" }
    filter {}

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
        "LightningGame/Assets/**",
        "LightningGame/Shaders/Shader.metal",
    }

    includedirs { "LightningCore", "ThirdParty", "ThirdParty/imgui", "ThirdParty/glfw/include", "ThirdParty/metal-cpp" }

    filter "system:macosx"
        cppdialect "C++23"
        staticruntime "On"
        
        files
        { 
            "LightningEditor/Source/main.cpp", 
            "LightningGame/Shaders/Shader.metal",
            "LightningGame/Assets/**",
        }

        filter { "system:macosx", "files:LightningGame/Assets/**" }
        buildaction "Resource"
        filter { "system:macosx", "files:LightningGame/Shaders/**" }
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
            ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.yourcompany.LightningGame"
        }

        buildoptions { "-std=c++23", "-stdlib=libc++" }
        linkoptions  { "-stdlib=libc++" }
    filter {}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    filter {}

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"
    filter {}