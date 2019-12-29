cmake_minimum_required(VERSION 3.15)
project(AsioTest)

include(ExternalProject)

#
# Create a target that represents downloading ASIO into the build tree
# The target's name is asio_install.
# Output: a variable called asio_include_path
#
ExternalProject_Add(asio_install
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
ExternalProject_Get_Property(asio_install SOURCE_DIR)
set(asio_include_path "${SOURCE_DIR}/asio/include")

#
# build an imported library target representing Asio and all its dependencies
# The target's public name shall be the namespace name Asio::Asio. Its private name is
# asio_lib.
# asio_lib shall depend on asio_install and consume asio_include_path
#
add_library(asio_lib IMPORTED GLOBAL INTERFACE)
target_include_directories(asio_lib INTERFACE "${asio_include_path}")
add_dependencies(asio_lib asio_install)
find_package(Threads)
target_link_libraries(asio_lib INTERFACE Threads::Threads)
add_library(Asio::Asio ALIAS asio_lib)

#
# define the executable
#
file(GLOB_RECURSE test1_sources LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")
add_executable(test1 ${test1_sources})
target_include_directories(test1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(test1 Asio::Asio)
