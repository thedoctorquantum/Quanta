add_library("Quanta" Source/Quanta/Quanta.h Source/Quanta/Log.cpp)

set_target_properties("Quanta" PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories("Quanta" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")