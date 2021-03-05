
#ifndef SFML_TEMPLATES_HPP
#define SFML_TEMPLATES_HPP

using namespace sf;

template <typename T>
std::string toString(T arg)
{
    std::stringstream ss;
    ss << arg;
    return ss.str();
}

#endif // SFML_FILENAME_HPP
