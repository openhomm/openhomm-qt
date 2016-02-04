import qbs 1.0

Project {
    CppApplication {
        name: "openhomm"
        Group {
            name: "Main files"
            files: ["main.cpp", "hrApplication.hpp", "hrApplication.cpp", "precompiled.hpp", "version.hpp", "precompiled.hpp.cpp"]
        }
        Group {
            name: "Core files"
            files: ["core/*.hpp", "core/*.cpp"]
        }

        Group {
            name: "Game files"
            files: ["game/*.hpp", "game/*.cpp"]
        }

        Group {
            name: "Render files"
            files: ["render/*.hpp", "render/*.cpp"]
        }

        Depends {
                name: "Qt"
                submodules: ["core", "widgets", "core-private", "opengl"]
        }

        Depends {
            name: "cpp"
        }

        cpp.cxxPrecompiledHeader: "precompiled.hpp"
        cpp.includePaths: [
            "core",
            "game",
            "render",
            "gui",
            "."
        ]

        cpp.cxxLanguageVersion: "c++11"
        cpp.staticLibraries: [
            "z",
            "GL" // Workaround
        ]
    }
}


