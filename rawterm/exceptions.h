#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

namespace rawterm {
    class KeypressError : public std::runtime_error {
       public:
        KeypressError(const std::string& msg) : std::runtime_error {msg} {}
    };
}  // namespace rawterm

#endif  // EXCEPTIONS_H
