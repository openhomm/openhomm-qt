import qbs

DynamicLibrary {
    name: "hrPcx"

    Depends {
        name: "Qt"
        submodules: ["core","gui"]
    }

    files: [
        "hrPcxHandler.cpp",
        "hrPcxHandler.hpp",
        "main.hpp",
        "main.cpp",
        "hrpcx.json"
    ]
}
