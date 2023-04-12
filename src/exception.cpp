#include <RenderToy/exception.h>

virtual const char *RenderToy::Exception::IRenderToyException::what() const noexcept
{
    return exception_what.c_str();
}

RenderToy::Exception::IRenderToyException::IRenderToyException(std::string exception_what_) noexcept
    : exception_what(exception_what_)
{
}

RenderToy::Exception::ImageSizeNotMatchException::ImageSizeNotMatchException(std::string exception_what_) noexcept
    : exception_what(exception_what_)
{
}
