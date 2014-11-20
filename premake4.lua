premake.gcc.cc = 'clang'
premake.gcc.cxx = 'clang++'

solution "woodwalk"
configurations { "Debug", "Release" }

project "woodwalk"
    kind "ConsoleApp"
    language "C++"
    files { "**.h", "**.hpp", "**.cpp", "*.inl" }
    excludes { "book/*" }
    links { 
        "sfml-graphics", "sfml-window", "sfml-audio",
        "sfml-network", "sfml-system"
     }
     buildoptions { "--std=c++11" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols", "ExtraWarnings" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }
