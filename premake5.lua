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
        externalincludedirs { "ThirdParty/metal-cpp", "ThirdParty/metal-cpp-extensions", "ThirdParty/stb"}
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
        ["Assets/*"] = {"LightningGame/Assets/*.png"},
    }

    files { "LightningGame/Source/**.h", "LightningGame/Source/**.cpp", "LightningGame/Shaders/**.metal",  "LightningGame/Assets/*.png"}

    includedirs { "ThirdParty", "LightningGame/Source", "LightningCore" }
    libdirs { "bin/%{cfg.buildcfg}" }
    links { "LightningCore" }

    filter "system:macosx"
    targetextension ".app"
    xcodebuildsettings {
        ["INFOPLIST_FILE"] = "Info.plist",
        ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.yourcompany.LightningEditor",
        ["ARCHS"] = "arm64",
        ["VALID_ARCHS"] = "arm64",
        ["ONLY_ACTIVE_ARCH"] = "YES",
        ["SKIP_INSTALL"] = "YES",
        ["ENABLE_BITCODE"] = "NO"
    }

    -- Enable profiling & debugging only for Debug builds
    filter "configurations:Debug"
        symbols "On"
        optimize "Off"
        xcodebuildsettings {
            ["CODE_SIGN_IDENTITY"] = "Sign to Run Locally",
            ["CODE_SIGN_STYLE"] = "Automatic",
            ["CODE_SIGNING_REQUIRED"] = "YES",
            ["CODE_SIGNING_ALLOWED"] = "YES",
            ["CODE_SIGN_ENTITLEMENTS"] = "Debug.entitlements",
            ["ENABLE_HARDENED_RUNTIME"] = "NO",
            ["DEBUG_INFORMATION_FORMAT"] = "dwarf-with-dsym"
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

        externalincludedirs {"ThirdParty/metal-cpp", "ThirdParty/metal-cpp-extensions", "LightningCore/", "ThirdParty/stb"}

    filter {}

project "LightningEditor"

    kind "WindowedApp"
    location "LightningEditor"

   vpaths 
    {
        ["Header Files/*"] = { "LightningEditor/Source/**.h"},
        ["Source Files/*"] = {"LightningEditor/Source/**.cpp"},
        ["Shaders/*"] = { "LightningGame/Shaders/**.metal"},
        ["Assets/*"] = {"LightningGame/Assets/*.png"},
    }

    files { "LightningEditor/Source/**.h", "LightningEditor/Source/**.cpp", "LightningGame/Shaders/**.metal", "LightningGame/Assets/*.png"}

    includedirs { "ThirdParty", "LightningEditor/Source", "LightningCore" }
    libdirs { "bin/%{cfg.buildcfg}" }
    links { "LightningCore" }

    filter "system:macosx"
    targetextension ".app"
    xcodebuildsettings {
        ["INFOPLIST_FILE"] = "Info.plist",
        ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.yourcompany.LightningEditor",
        ["ARCHS"] = "arm64",
        ["VALID_ARCHS"] = "arm64",
        ["ONLY_ACTIVE_ARCH"] = "YES",
        ["SKIP_INSTALL"] = "YES",
        ["ENABLE_BITCODE"] = "NO"
    }

    -- Enable profiling & debugging only for Debug builds
    filter "configurations:Debug"
        symbols "On"
        optimize "Off"
        xcodebuildsettings {
            ["CODE_SIGN_IDENTITY"] = "Sign to Run Locally",
            ["CODE_SIGN_STYLE"] = "Automatic",
            ["CODE_SIGNING_REQUIRED"] = "YES",
            ["CODE_SIGNING_ALLOWED"] = "YES",
            ["CODE_SIGN_ENTITLEMENTS"] = "Debug.entitlements",
            ["ENABLE_HARDENED_RUNTIME"] = "NO",
            ["DEBUG_INFORMATION_FORMAT"] = "dwarf-with-dsym"
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

        externalincludedirs {"ThirdParty/metal-cpp", "ThirdParty/metal-cpp-extensions", "LightningCore/", "ThirdParty/stb"}

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

    
