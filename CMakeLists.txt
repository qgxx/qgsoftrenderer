cmake_minimum_required (VERSION 3.17) 
project (qgsoftrenderer)

set (CMAKE_C_STANDARD 99)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${PROJECT_SOURCE_DIR}/external/include/)
include_directories(${PROJECT_SOURCE_DIR}/renderer/)
include_directories(${PROJECT_SOURCE_DIR}/renderer/interface)
include_directories(${PROJECT_SOURCE_DIR}/renderer/platform)
include_directories(${PROJECT_SOURCE_DIR}/renderer/textures)
include_directories(${PROJECT_SOURCE_DIR}/renderer/utils)

find_library(SDL2_LIBRARY NAMES SDL2 PATHS ${PROJECT_SOURCE_DIR}/external/lib/ NO_CMAKE_FIND_ROOT_PATH NO_SYSTEM_ENVIRONMENT_PATH)
find_library(SDL2main_LIBRARY NAMES SDL2main PATHS ${PROJECT_SOURCE_DIR}/external/lib/ NO_CMAKE_FIND_ROOT_PATH NO_SYSTEM_ENVIRONMENT_PATH)
find_library(tbb12_LIBRARY NAMES tbb12 PATHS ${PROJECT_SOURCE_DIR}/external/lib/ NO_CMAKE_FIND_ROOT_PATH NO_SYSTEM_ENVIRONMENT_PATH)
find_library(glm_LIBRARY NAMES glm PATHS ${PROJECT_SOURCE_DIR}/external/lib/ NO_CMAKE_FIND_ROOT_PATH NO_SYSTEM_ENVIRONMENT_PATH)
find_library(assimp-vc143-mt_LIBRARY NAMES assimp-vc143-mt PATHS ${PROJECT_SOURCE_DIR}/external/lib/ NO_CMAKE_FIND_ROOT_PATH NO_SYSTEM_ENVIRONMENT_PATH)

link_directories(${PROJECT_SOURCE_DIR}/external/bin/)
file(COPY ${PROJECT_SOURCE_DIR}/assets DESTINATION ${PROJECT_SOURCE_DIR}/build/Release)
file(COPY ${PROJECT_SOURCE_DIR}/external/bin/ DESTINATION ${PROJECT_SOURCE_DIR}/build/Release/)

add_subdirectory(${PROJECT_SOURCE_DIR}/renderer)
add_subdirectory(${PROJECT_SOURCE_DIR}/test)

MESSAGE( STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY} )
MESSAGE( STATUS "SDL2main_LIBRARY: " ${SDL2main_LIBRARY} )
MESSAGE( STATUS "tbb12_LIBRARY: " ${tbb12_LIBRARY} )
MESSAGE( STATUS "glm_LIBRARY: " ${glm_LIBRARY} )
MESSAGE( STATUS "assimp-vc143-mt_LIBRARY: " ${assimp-vc143-mt_LIBRARY} )