#ifndef P01_PARSEREXCEPTION_HPP
#define P01_PARSEREXCEPTION_HPP

#include <exception>
#include <string>

class ParserException : public std::exception {
    public:
        explicit ParserException(std::string  message);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        std::string m;
};


#endif //P01_PARSEREXCEPTION_HPP
