cmake_minimum_required(VERSION 3.0.0)
project(ideal-gas VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Debug)

# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(CTest)
enable_testing()

include(FetchContent)

FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
)

# Adds Catch2 testing library
FetchContent_GetProperties(catch2)
if(NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_library(catch2 INTERFACE)
    target_include_directories(catch2 INTERFACE ${catch2_SOURCE_DIR}/single_include)
endif()

get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

list(APPEND CORE_SOURCE_FILES   src/*)

list(APPEND SOURCE_FILES    ${CORE_SOURCE_FILES}
                            )

list(APPEND TEST_FILES )

FIND_PACKAGE( Boost COMPONENTS program_options; serialization REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

ci_make_app(
        APP_NAME        ideal-gas-simulation
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         apps/main.cc ${SOURCE_FILES}
        INCLUDES        include
        LIBRARIES       ${Boost_LIBRARIES}
)

# ci_make_app(
#         APP_NAME        ideal-gas-test
#         CINDER_PATH     ${CINDER_PATH}
#         SOURCES         tests/test_main.cc ${SOURCE_FILES} ${TEST_FILES}
#         INCLUDES        include
#         LIBRARIES       catch2 ${Boost_LIBRARIES}
# )

add_executable(ideal-gas apps/main.cpp ${CORE_SOURCE_FILES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
