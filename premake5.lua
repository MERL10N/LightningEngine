-- Lightning Engine Premake Setup

workspace "LightningEngine"
    configurations { "Debug", "Release" }

    -- Global compiler settings
    language "C++"
    cppdialect "C++23"
    architecture "ARM64"

    -- Platform-specific settings
    filter "system:macosx"
        toolset "clang"
        defines { "PLATFORM_MAC" }
        buildoptions { "-std=c++23 -fobjc-arc" }
        externalincludedirs { "ThirdParty/metal-cpp", "ThirdParty/metal-cpp-extensions" }

    filter {}

-- Lightning Application
project "LightningGame"
    kind "WindowedApp"
    location "LightningGame"

    vpaths 
    {
        ["Header Files/*"] = { "LightningGame/Source/**.h"},
        ["Source Files/*"] = {"LightningGame/Source/**.cpp"},
        ["Shaders/*"] = { "LightningGame/Shaders/**.metal"},
    }

    files { "LightningGame/Source/**.h", "LightningGame/Source/**.cpp", "LightningGame/Shaders/**.metal" }

    includedirs { "ThirdParty", "LightningGame/Source", "LightningCore" }
    libdirs { "bin/%{cfg.buildcfg}" }
    links { "LightningCore" }

    filter "system:macosx"
        targetextension ".app"
        xcodebuildsettings {
            ["INFOPLIST_FILE"] = "Info.plist",  -- Explicitly set the Info.plist path
            ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.yourcompany.LightningGame",
            ["ARCHS"] = "arm64",
            ["VALID_ARCHS"] = "arm64",
            ["ONLY_ACTIVE_ARCH"] = "YES",
            ["CODE_SIGN_IDENTITY"] = "-",
            ["CODE_SIGN_STYLE"] = "Automatic",
            ["CODE_SIGNING_REQUIRED"] = "NO",
            ["CODE_SIGNING_ALLOWED"] = "NO",
            ["SKIP_INSTALL"] = "YES",
            ["ENABLE_BITCODE"] = "NO"
        }

        -- Link Apple frameworks
        linkoptions { 
            "-framework Foundation",
            "-framework Metal",
            "-framework MetalKit",
            "-framework QuartzCore",
            "-framework Cocoa",
            "-framework AppKit"
        }

        externalincludedirs {"ThirdParty/metal-cpp", "ThirdParty/metal-cpp-extensions", "LightningCore/"}

    filter {}

project "LightningEditor"
kind "WindowedApp"
location "LightningEditor"

    vpaths 
    {
        ["Header Files/*"] = { "LightningEditor/Source/**.h"},
        ["Source Files/*"] = {"LightningaEditor/Source/**.cpp"},
    }

    files { "LightningEditor/Source/**.h", "LightningEditor/Source/**.cpp" }

    includedirs { "ThirdParty", "LightningEditor/Source", "LightningCore" }
    libdirs { "bin/%{cfg.buildcfg}" }
    links { "LightningCore" }

    filter "system:macosx"
        targetextension ".app"
        xcodebuildsettings {
            ["INFOPLIST_FILE"] = "Info.plist",  -- Explicitly set the Info.plist path
            ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.yourcompany.LightningEditor",
            ["ARCHS"] = "arm64",
            ["VALID_ARCHS"] = "arm64",
            ["ONLY_ACTIVE_ARCH"] = "YES",
            ["CODE_SIGN_IDENTITY"] = "-",
            ["CODE_SIGN_STYLE"] = "Automatic",
            ["CODE_SIGNING_REQUIRED"] = "NO",
            ["CODE_SIGNING_ALLOWED"] = "NO",
            ["SKIP_INSTALL"] = "YES",
            ["ENABLE_BITCODE"] = "NO"
        }

        -- Link Apple frameworks
        linkoptions { 
            "-framework Foundation",
            "-framework Metal",
            "-framework MetalKit",
            "-framework QuartzCore",
            "-framework Cocoa",
            "-framework AppKit"
        }

        externalincludedirs {"ThirdParty/metal-cpp", "ThirdParty/metal-cpp-extensions", "LightningCore/", "ThirdParty/imgui"}

    filter {}

-- Engine Core
project "LightningCore"
    kind "StaticLib"
    staticruntime "on"
    location "LightningCore"
    files { "LightningCore/**.h", "LightningCore/**.cpp" }
    filter "system:macosx"
        xcodebuildsettings {
            ["SKIP_INSTALL"] = "YES",
            ["ENABLE_BITCODE"] = "NO",
            ["CLANG_ENABLE_MODULES"] = "NO"
        }

    
