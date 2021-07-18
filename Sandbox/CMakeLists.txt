add_executable("Sandbox" Source/Sandbox/main.cpp)

set_target_properties("Sandbox" PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries("Sandbox" PUBLIC "Quanta")