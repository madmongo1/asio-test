macro(build_asio)

    set(deps_source_dir ${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies)
    set(deps_binary_dir ${CMAKE_CURRENT_BINARY_DIR}/cmake/dependencies)
    set(deps_include_path)
    set(deps_prefix ${deps_binary_dir})

    if (DEFINED CMAKE_TOOLCHAIN_FILE)
        list(APPEND child_cmake_args "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
    endif()
    if (DEFINED CMAKE_GENERATOR)
        list(APPEND child_cmake_args "-G" "${CMAKE_GENERATOR}")
    endif()
    #message(FATAL_ERROR "${CMAKE_COMMAND}"  "${child_cmake_args} ${deps_source_dir}")
    execute_process(
            COMMAND ${CMAKE_COMMAND}  ${child_cmake_args} -H${deps_source_dir} -B${deps_binary_dir}
            COMMAND_ECHO STDOUT
    )
    execute_process(
            COMMAND ${CMAKE_COMMAND} --build ${deps_binary_dir} --target asio_library
    )

    add_library(asio_lib IMPORTED GLOBAL INTERFACE)
    target_include_directories(asio_lib INTERFACE "${deps_prefix}/include")
    find_package(Threads)
    target_link_libraries(asio_lib INTERFACE Threads::Threads)
    add_library(Asio::Asio ALIAS asio_lib)
endmacro()

macro(build_dependencies)
    build_asio()
endmacro()
