file(GLOB_RECURSE SOURCE Source/*.c Source/*.h Source/*.cpp Source/*.hpp)

add_library("${CMAKE_PROJECT_NAME}" STATIC "${SOURCE}")

target_include_directories("${CMAKE_PROJECT_NAME}" 
    PUBLIC 
    
    "${CMAKE_CURRENT_SOURCE_DIR}/Include"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/angelscript-mirror/sdk/angelscript/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/glm"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/External/entt"
)

add_subdirectory("External")

target_precompile_headers("${CMAKE_PROJECT_NAME}" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/Source/Utilites/Precompiled.h")

target_link_libraries("${CMAKE_PROJECT_NAME}" 
    PUBLIC 
    
    "angelscript"
    "imgui"
    
    PRIVATE 

    "assimp"
    "glad"
    "glfw"
    "sndfile"
    "OpenAL"
    "stb_image"
)