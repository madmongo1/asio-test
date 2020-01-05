cmake_minimum_required(VERSION 3.15)
project(Dependencies)

include(ExternalProject)

set_property(DIRECTORY PROPERTY EP_PREFIX "${CMAKE_CURRENT_BINARY_DIR}")

execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/include")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/lib")
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/bin")
#
# Create a target that represents downloading ASIO into the build tree
# The target's name is asio_install.
# Output: a variable called asio_include_path
#
ExternalProject_Add(asio_build
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG master
        GIT_SHALLOW On
        GIT_PROGRESS On

        UPDATE_DISCONNECTED On
        PATCH_COMMAND ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        )
ExternalProject_Get_Property(asio_build SOURCE_DIR)
set(asio_include_path "${SOURCE_DIR}/asio/include")

add_custom_target(asio_library
        DEPENDS asio_build
        VERBATIM
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${asio_include_path}/asio "${CMAKE_CURRENT_BINARY_DIR}/include/asio"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${asio_include_path}/asio.hpp "${CMAKE_CURRENT_BINARY_DIR}/include"
        )