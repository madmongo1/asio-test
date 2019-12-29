#include <iostream>
#include <asio.hpp>
#include <chrono>
#include <future>
#include <experimental/coroutine>

using namespace std::literals;
namespace net = asio;

std::mutex emit_mutex;
template<class...Stuff>
void echo(Stuff&&...stuff)
{
    auto l = std::unique_lock(emit_mutex);
    (std::cout << ... << stuff);
    std::cout << '\n';
}

template<class StringLike>
auto underline(StringLike input)
{
    auto buffer = std::string(std::forward<StringLike>(input));
    auto len = buffer.size();
    buffer += '\n';
    buffer.append(len, '=');
    return buffer;
}

auto run() -> net::awaitable<void>
{
    auto exec = co_await net::this_coro::executor;

    echo("A");

    auto t = net::high_resolution_timer (exec);
    t.expires_after(500ms);
    co_await t.async_wait(net::use_awaitable);

    echo("B");
}

class io_environment
{
    net::io_context ioc;
    std::exception_ptr ep;
public:
    static constexpr auto classname() { return "io_environment"; }

    net::io_context::executor_type exec = ioc.get_executor();

    void notify_complete(std::exception_ptr ep)
    {
        this->ep = ep;
    }

    void run()
    {
        ioc.run();
        if (ep)
            std::throw_with_nested(ep);
    }

};

struct system_environment
{
    static constexpr auto classname() { return "system_environment"; }

    net::system_executor exec;

    void notify_complete(std::exception_ptr ep)
    {
        if (ep)
            p.set_exception(ep);
        else
            p.set_value();
    }

    void run()
    {
        f.wait();
    }

private:
    std::promise<void> p;
    std::future<void> f = p.get_future();
};

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

#define USE_SYSTEM_EXECUTOR 1

int main()
{
    prog(system_environment());
    prog(io_environment());
    return 0;
}
