#include <stdexcept>

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string &message) : runtime_error(message) {}
};
