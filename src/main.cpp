#include "boilerplate.hpp"
#include <experimental/coroutine>
#include <chrono>

#define TRY_AWAITABLE_DEFAULT 0

using namespace std::literals;

auto run() -> net::awaitable<void> {
    auto exec = co_await
    net::this_coro::executor;

    echo("A");

    auto t1 = net::high_resolution_timer(exec);
    t1.expires_after(500ms);
    co_await
    t1.async_wait(net::use_awaitable);

#if TRY_AWAITABLE_DEFAULT
    /*
     this does not compile on clang 9.0 with -fcoroutines -stdlib=libc++
     */

    auto t2 = net::use_awaitable.as_default_on(net::high_resolution_timer(exec));
    t2.expires_after(500ms);
    co_await t2.async_wait();

#endif

    echo("B");
}

template<class Environment>
void prog(Environment env) {
    echo(underline(env.classname()));

    net::co_spawn(env.exec,
                  [] {
                      return run();
                  },
                  [&](std::exception_ptr ep) {
                      echo("done\n");
                      env.notify_complete(ep);
                  });

    env.run();
}

int main() {
    prog(system_environment());
    prog(io_environment());
    return 0;
}
