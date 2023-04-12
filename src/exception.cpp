#include <RenderToy/exception.h>

const char *RenderToy::Exception::IRenderToyException::what() const noexcept
{
    return exception_what.c_str();
}

RenderToy::Exception::IRenderToyException::IRenderToyException(const std::string &exception_what_) noexcept
    : exception_what(exception_what_)
{
}

RenderToy::Exception::ImageSizeNotMatchException::ImageSizeNotMatchException(const std::string &exception_what_) noexcept
    : IRenderToyException(exception_what_)
{
}
