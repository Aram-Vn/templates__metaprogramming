#include <iostream>
#include <type_traits>

// SFINAE
// std::integral_constant
// concept <----------------important ???
// require <----------------important ???
// declval<T>()
// type_traits
// std::remove_reference, std::remove_const, std::remove_cv, std::remove_volatile

//-------------------------------------//
template <typename T>
std::enable_if<std::is_integral_v<T>, T>::type sum(T a, T b) // only for integral types
{
    return a + b;
}

template <typename T>
std::enable_if<!std::is_integral_v<T>, T>::type sum(T a, T b) // other types
{
    return a + b;
}
//-------------------------------------//

//-------------------------------------_is_same_-------------------------------------***//
template <typename T, typename U>
struct Is_Same
{
    static const bool val = false;
};

template <typename T>
struct Is_Same<T, T>
{
    static const bool val = true;
};

// կամ ժառանգելով՝

template <typename T, typename U>
struct isSame : std::false_type
{
};

template <typename T>
struct isSame<T, T> : std::true_type
{
};
//-------------------------------------// is_same

//*************_enable_if_-------------------------------------//
template <bool val, typename T>
struct enable_if
{
};

template <typename T>
struct enable_if<true, T>
{
    using types = T;
};
//-------------------------------------// enable_if

//-------------------------------------_is_integral_-------------------------------------//
template <typename T>
void is_integral()
{
    if constexpr (std::is_integral<T>::value)
    {
        std::cout << "yes" << std::endl;
    }
    else
    {
        std::cout << "no" << std::endl;
    }
}
//-------------------------------------// is_integral

//-------------------------------------_is_defoult_constructable_-------------------------------------//
template <typename T, class = void>
struct is_defoult_constructable : std::false_type
{
};

template <typename T>
struct is_defoult_constructable<T, std::void_t<decltype(T())>> : std::true_type
{
};

struct AA
{
    AA() = delete;
};
//-------------------------------------// is_defoult_constructable

//-------------------------------------_always const reference_-------------------------------------//
template <typename T>
void foo(const typename std::remove_cv<typename std::remove_reference<T>::type>::type& val)
{
    // some interactions ...
}
//-------------------------------------// always const reference

int main()
{
    int n = 5;
    int m = 7;

    std::cout << sum(n, m) << std::endl;

    double x = 0.5;
    double y = 1.25;

    std::cout << sum(x, y) << std::endl;

    if (is_defoult_constructable<int>::value)
    {
        std::cout << "yes" << std::endl;
    }
    else
    {
        std::cout << "no" << std::endl;
    }

    if (is_defoult_constructable<AA>::value)
    {
        std::cout << "yes" << std::endl;
    }
    else
    {
        std::cout << "no" << std::endl;
    }
}