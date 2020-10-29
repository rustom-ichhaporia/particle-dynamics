cmake_minimum_required(VERSION 3.0.0)
project(ideal-gas VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# include(CTest)
# enable_testing()

# Warning flags
if(MSVC)
    # warning level 3 and all warnings as errors
    add_compile_options(/W3 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wpedantic -Werror)
endif()

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent.git
  GIT_TAG v3.7.3)

FetchContent_GetProperties(json)
FetchContent_MakeAvailable(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

find_package( Boost 1.54 REQUIRED COMPONENTS system filesystem )
list( APPEND CINDER_LIBS_DEPENDS ${Boost_LIBRARIES} )
list( APPEND CINDER_INCLUDE_SYSTEM_PRIVATE ${Boost_INCLUDE_DIRS} )

get_filename_component(CINDER_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../" ABSOLUTE)
get_filename_component(APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE)

include("${CINDER_PATH}/proj/cmake/modules/cinderMakeApp.cmake")

list(APPEND CORE_SOURCE_FILES   
    src/core/particle_container.cc
    src/core/particle.cc
)

list(APPEND VISUALIZER_SOURCE_FILES  
    src/visualizer/ideal_gas_visualizer.cc

)

list(APPEND TEST_FILES      tests/test_main.cc
                            tests/test_model.cc)

ci_make_app(
        APP_NAME        ideal-gas-visualizer
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         apps/ideal_gas_visualizer_app.cpp ${CORE_SOURCE_FILES} ${VISUALIZER_SOURCE_FILES}
        INCLUDES        include
        LIBRARIES       nlohmann_json::nlohmann_json
)

ci_make_app(
        APP_NAME        ideal-gas-test
        CINDER_PATH     ${CINDER_PATH}
        SOURCES         tests/test_main.cc ${CORE_SOURCE_FILES} ${TEST_FILES}
        INCLUDES        include
        LIBRARIES       Catch2 
)
# set_target_properties(ideal-gas-visualizer
#     PROPERTIES
#         CXX_STANDARD 17
#         CXX_STANDARD_REQUIRED YES
#         CXX_EXTENSIONS NO
# )
# add_library(ideal-gas-visualizer nlohmann_json::nlohmann_json)

target_include_directories(ideal-gas-visualizer PRIVATE include)#; nlohmann_json::nlohmann_json_cmake_fetchcontent)#nlohmann_json::nlohmann_json)
# target_link_libraries(ideal-gas-visualizer PRIVATE nlohmann_json::nlohmann_json)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
