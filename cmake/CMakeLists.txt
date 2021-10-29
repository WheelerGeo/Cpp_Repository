cmake_minimum_required(VERSION 3.8)

project(cmake)

set(CMAKE_CXX_STANDARD 14)
set(PROJECT_SOURCE_DIR ../)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)
aux_source_directory(${PROJECT_SOURCE_DIR}/src NWK_SRCS_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/json JSON_SRCS_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/yaml YAML_SRCS_LIST)

add_library(nwk_static STATIC ${NWK_SRCS_LIST})
add_library(nwk_shared SHARED ${NWK_SRCS_LIST})
add_library(json_static STATIC ${JSON_SRCS_LIST})
add_library(json_shared SHARED ${JSON_SRCS_LIST})
add_library(yaml_static STATIC ${YAML_SRCS_LIST})
add_library(yaml_shared SHARED ${YAML_SRCS_LIST})

find_package(Threads)



if(TARGET)
    add_executable(${TARGET} ${PROJECT_SOURCE_DIR}/demo/${TARGET}.cpp)
    target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT} nwk_static yaml_static json_static)
else()
    # Find all main*.cpp files and store in list mains
    file(GLOB_RECURSE DIR_TARGET_LIST "${PROJECT_SOURCE_DIR}/demo/*.cpp")
    foreach(DIR_TARGET ${DIR_TARGET_LIST})
        # Get file name without directory
        get_filename_component(DIR_TARGET_NAME ${DIR_TARGET} NAME_WE)
        add_executable(${DIR_TARGET_NAME} ${DIR_TARGET})
        target_link_libraries(${DIR_TARGET_NAME} ${CMAKE_THREAD_LIBS_INIT} nwk_static yaml_static json_static)
    endforeach()
endif()


