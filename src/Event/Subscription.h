#pragma once

#include <cstddef>   // std::size_t
#include <utility>   // std::exchange

/**
 * Dev Note:
 * It's an attribute, a note to the compiler written in [[...]].
 * It means "the caller shouldn't ignore this function's return value."
 * If someone calls the function and throws the result away, the compiler gives a warning.
 * 
 * 1) Subscribe returns a temporary Subscription.
 * 2) Nobody stores it, so it's destroyed at the end of that line.
 * 3) Its destructor calls Unsubscribe.
 * 4) The handler is removed right after being added and never fires. There's no crash and no error. It just doesn't work.
 * 
 * [[nodiscard]] catches exactly that.
 */
class [[nodiscard]] Subscription
{
public:
    using Token = std::size_t;

    explicit Subscription(Token id) : id_(id) {}
    Subscription(Subscription&& o) noexcept : id_(std::exchange(o.id_, 0)) {}
    ~Subscription();
    Subscription& operator=(Subscription&&) = delete;
    Subscription(const Subscription&) = delete;

private:
    Token id_;

};
