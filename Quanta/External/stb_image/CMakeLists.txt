add_library("stb_image" "Source/stb_image.cpp" "Include/stb_image.h")

set_target_properties("stb_image" PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories("stb_image" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include")