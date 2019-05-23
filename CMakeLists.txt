cmake_minimum_required(VERSION 2.8)

project(Demo_SK)

include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#64位静态链接库
# link_libraries(assimp-vc140-mt.lib)
# link_libraries(glfw3_64.lib)

#32位静态链接库
# link_libraries(glfw3.lib)
# link_libraries(assimp.lib)


IF( CMAKE_SIZEOF_VOID_P EQUAL 8)
    #64位静态链接库
    link_libraries(assimp-vc140-mt.lib)
    link_libraries(glfw3_64.lib)
ELSE( CMAKE_SIZEOF_VOID_P EQUAL 8)
    #32位静态链接库
    link_libraries(glfw3.lib)
    link_libraries(assimp.lib)
ENDIF( CMAKE_SIZEOF_VOID_P EQUAL 8)



aux_source_directory(. DIR_SRCS)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)   
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin) 

add_executable(Demo_SK ${DIR_SRCS})