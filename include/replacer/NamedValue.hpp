#ifndef LIBREPLACER_NAMEDVALUE_HPP
#define LIBREPLACER_NAMEDVALUE_HPP

namespace replacer
{

template<class T, class Tag>
class NamedValue
{
public:
    explicit NamedValue(const T &value)
        : value_ {value}
    {}

    operator T () const { return value_; }
    const T& operator * () const
    {
        return value_;
    }

    using type = T;

private:
    T value_;
};

} // namespace replacer

#endif // LIBREPLACER_NAMEDVALUE_HPP