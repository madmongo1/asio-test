cmake_minimum_required(VERSION 3.15)
project(AsioTest)

include(ExternalProject)

include(cmake/build_dependencies.cmake)
build_dependencies()
#
# Create a target that represents downloading ASIO into the build tree
# The target's name is asio_install.
# Output: a variable called asio_include_path
#
#

# build an imported library target representing Asio and all its dependencies
# The target's public name shall be the namespace name Asio::Asio. Its private name is
# asio_lib.
# asio_lib shall depend on asio_install and consume asio_include_path
#

#
# define the executable
#
file(GLOB_RECURSE test1_sources LIST_DIRECTORIES false CONFIGURE_DEPENDS "src/*.cpp" "src/*.hpp")
add_executable(test1 ${test1_sources})
target_include_directories(test1 PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(test1 Asio::Asio)
