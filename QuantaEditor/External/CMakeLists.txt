add_library(
    "ImGuizmo" 
    
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/ImGuizmo.cpp"    
)

target_include_directories("imgui" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/ImGuizmo/")