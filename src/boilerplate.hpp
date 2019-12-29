#pragma once

#include "config.hpp"
#include <string>
#include <mutex>
#include <future>
#include <iostream>
#include <ciso646>

extern std::mutex emit_mutex;

template<class...Stuff>
void echo(Stuff&&...stuff)
{
    auto l = std::unique_lock(emit_mutex);
    (std::cout << ... << stuff);
    std::cout << '\n';
}

template<class T>
auto get_length(T&& x)
-> std::enable_if_t<not std::is_convertible_v<T&&, const char*>, std::size_t>
{
    return x.size();
}

template<class T>
auto
get_length(T&& x)
-> std::enable_if_t<std::is_convertible_v<T&&, const char*>, std::size_t>
{
    return std::strlen(x);
};

struct underliner_base
{
    static thread_local std::string buffer;
};

template<class T>
struct underliner : underliner_base
{
    underliner(T& thing) : thing(thing) {}

    friend auto operator<<(std::ostream& os, underliner u) -> std::ostream&
    {
        u.buffer.resize(get_length(u.thing), '=');
        os << u.thing << '\n' << u.buffer;
        return os;
    }

    T& thing;
};

template<class StringLike>
auto underline(StringLike&& input)
{
    return underliner<StringLike>(input);
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

