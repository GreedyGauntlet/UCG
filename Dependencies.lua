IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/vendor/flora/Flora/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/vendor/flora/Flora/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/vendor/flora/Flora/vendor/box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/vendor/flora/Flora/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/vendor/flora/Flora/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/vendor/flora/Flora/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/vendor/flora/Flora/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/vendor/flora/Flora/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/vendor/flora/Flora/vendor/entt/include"
IncludeDir["mono"] = "%{wks.location}/vendor/flora/Flora/vendor/mono/include"
IncludeDir["filewatch"] = "%{wks.location}/vendor/flora/Flora/vendor/filewatch"
IncludeDir["openAL"] = "%{wks.location}/vendor/flora/Flora/vendor/openAL/include"
IncludeDir["msdfgen"] = "%{wks.location}/vendor/flora/Flora/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["msdfatlas"] = "%{wks.location}/vendor/flora/Flora/vendor/msdf-atlas-gen/msdf-atlas-gen"

LibraryDir = {}
LibraryDir["mono"] = "%{wks.location}/vendor/flora/Flora/vendor/mono/lib/%{cfg.buildcfg}"
LibraryDir["openAL"] = "%{wks.location}/vendor/flora/Flora/vendor/openAL/libs/Win64"

Library = {}
Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"

-- Windows only
Library["WinSock"] = "Ws2_32.lib"
Library["Winmm"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["WinBCrypt"] = "Bcrypt.lib"
Library["Shell32"] = "Shell32.lib"

-- OpenAL: note that we're just gonna use the win32 build
Library["openAL"] = "%{LibraryDir.openAL}/OpenAL32.lib"
Library["efx"] = "%{LibraryDir.openAL}/EFX-Util_MT/EFX-Util.lib"