#include <exception>
#include <string>

namespace RenderToy::Exception
{
    class IRenderToyException : public std::exception
    {
    protected:
        std::string exception_what;

    public:
        IRenderToyException(const std::string &exception_what_) noexcept;
        virtual const char *what() const noexcept;
    };

    class ImageSizeNotMatchException : public IRenderToyException
    {
    public:
        ImageSizeNotMatchException(const std::string &exception_what_) noexcept;
    };
}