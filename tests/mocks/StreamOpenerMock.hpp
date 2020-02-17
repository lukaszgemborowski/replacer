#ifndef LIBREPLACER_TESTS_MOCKS_STREAMOPENERMOCK_HPP
#define LIBREPLACER_TESTS_MOCKS_STREAMOPENERMOCK_HPP

#include <replacer/fs.hpp>
#include <string>
#include <map>
#include <sstream>

namespace replacer::mocks
{

class StreamOpenerMock
{
public:
    StreamOpenerMock(std::map<fs::path, std::string> streams)
        : streams {streams}
    {}

    template<class T>
    std::stringstream operator()(T path)
    {
        return std::stringstream{streams[path]};
    }

private:
    std::map<fs::path, std::string> streams;
};

} // namespace replacer::mocks

#endif // LIBREPLACER_TESTS_MOCKS_STREAMOPENERMOCK_HPP