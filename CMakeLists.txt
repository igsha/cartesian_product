cmake_minimum_required(VERSION 3.12)
project(dirprod VERSION 1.0.0 LANGUAGES CXX DESCRIPTION "Implementation of direct product iterator")

include(CMakePackageConfigHelpers)
include(CTest)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -pedantic -Wextra)

find_package(Boost REQUIRED)

add_library(dirprod INTERFACE)
target_include_directories(dirprod INTERFACE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_link_libraries(dirprod INTERFACE Boost::boost)

install(TARGETS dirprod EXPORT ${PROJECT_NAME}-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${PROJECT_NAME}-targets DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION})

write_basic_package_version_file(${PROJECT_NAME}-config-version.cmake COMPATIBILITY AnyNewerVersion)
configure_file(config.cmake.in ${PROJECT_NAME}-config.cmake @ONLY)
install(FILES
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION}
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

list(APPEND CPACK_GENERATOR TGZ)
set(CPACK_PACKAGE_CONTACT "igsha")
include(CPack)
