cmake_minimum_required(VERSION 3.0.0)
project(ideal-gas VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(CTest)
enable_testing()

# Warning flags
if(MSVC)
    # warning level 3 and all warnings as errors
    add_compile_options(/W3 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wpedantic -Werror)
endif()

include(FetchContent)

# FetchContent_MakeAvailable was not added until CMake 3.14
if(${CMAKE_VERSION} VERSION_LESS 3.14)
    include(cmake/add_FetchContent_MakeAvailable.cmake)
endif()

# FetchContent_Declare(
#         catch2
#         GIT_REPOSITORY https://github.com/catchorg/Catch2.git
# )

# # Adds Catch2 testing library
# FetchContent_GetProperties(catch2)
# if(NOT catch2_POPULATED)
#     FetchContent_Populate(catch2)
#     add_library(catch2 INTERFACE)
#     target_include_directories(catch2 INTERFACE ${catch2_SOURCE_DIR}/single_include)
# endif()

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

list(APPEND CORE_SOURCE_FILES   )#src/.cpp)

list(APPEND VISUALIZER_SOURCE_FILES  
    src/visualizer/ideal_gas_visualizer.cc

)

list(APPEND TEST_FILES      tests/test_main.cc
                            tests/test_model.cc)

# add_executable(ideal-gas /Users/rustomichhaporia/GitHub/Cinder/my-projects/ideal-gas-rustom-ichhaporia/apps/main.cpp ${CORE_SOURCE_FILES})

ci_make_app(
        APP_NAME        ideal-gas-simulation
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         apps/ideal_gas_visualizer_app.cpp ${SOURCE_FILES} ${VISUALIZER_SOURCE_FILES}
        INCLUDES        include
)

ci_make_app(
        APP_NAME        ideal-gas-test
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         tests/test_main.cc ${SOURCE_FILES} ${TEST_FILES}
        INCLUDES        include
        LIBRARIES       Catch2 
)

target_include_directories(ideal-gas-simulation PRIVATE include)
# target_link_libraries(ideal-gas-test Catch2::Catch2)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
