import qbs

DynamicLibrary {
    name: "hrDef"

    Depends {
        name: "Qt"
        submodules: ["core","gui"]
    }

    files: [
        "hrDefHandler.cpp",
        "hrDefHandler.hpp",
        "main.hpp",
        "main.cpp",
        "hrdef.json"
    ]
}
