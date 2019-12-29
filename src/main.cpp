#include "boilerplate.hpp"
#include <experimental/coroutine>
#include <chrono>

using namespace std::literals;

auto run() -> net::awaitable<void>
{
    auto exec = co_await net::this_coro::executor;

    echo("A");

    auto t = net::high_resolution_timer (exec);
    t.expires_after(500ms);
    co_await t.async_wait(net::use_awaitable);

    echo("B");
}

template<class Environment>
void prog(Environment env)
{
    echo(underline(env.classname()));

    net::co_spawn(env.exec, []{ return run(); }, [&](std::exception_ptr ep)
    {
        echo("done\n");
        env.notify_complete(ep);
    });

    env.run();
}

int main()
{
    prog(system_environment());
    prog(io_environment());
    return 0;
}
