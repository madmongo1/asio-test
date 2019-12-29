# asio-test
A test of new asio features

This tiny program is to test the new asio feature of `use_awaitable.as_default_on`

To build with clang:

cmake against a coroutine-enabled toolchain, or set `CMAKE_CXX_FLAGS="-fcoroutines -stdlib=libc++ -std=c++2a"`


