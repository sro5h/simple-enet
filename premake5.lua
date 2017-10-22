-- premake5.lua
workspace "app"
    configurations { "Debug", "Release" }
    if _ACTION == nil then
        _ACTION = "gmake"
    end

project "app"
    kind "ConsoleApp"
    language "C++"
    targetdir "."
    includedirs { "." }

    files { "**.hpp", "**.h", "**.cpp" }
    removefiles { "./test/*" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        links { "enet" }

-- Clean function
newaction {
    trigger = "clean",
    description = "clean the software",
    execute = function ()
        print("cleaning...")
        os.rmdir("./bin")
        os.rmdir("./obj")
        os.remove("./Makefile")
        os.remove("./*.make")
        os.remove("./app")
        os.remove("./tags")
        print("done.")
    end
}
