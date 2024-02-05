#include <algorithm>
#include <any>
#include <cstddef>
#include <exception>
#include <iostream>
#include <string>

//-------------------------helper functions-------------------------//
template <typename T>
void* __copy(void* ptr)
{
    if (!ptr)
    {
        return nullptr;
    }

    return new T(*static_cast<T*>(ptr));
}

template <typename T>
void __destroy(void* ptr)
{
    delete static_cast<T*>(ptr);
}

template <typename T>
const std::type_info& __get_type()
{
    return typeid(T);
}
//------------------------------------------------// helpers

//-----------------------error message--------------------------------//
class bad_any_cast : public std::exception
{
public:
    const char* what() const noexcept override { return "bad any err!"; }
};

//-----------------------class any-----------------//
class any
{
public:
    template <typename T>
    friend const T& any_cast(const any& ob);

    template <typename T>
    friend const T* any_cast(const T* ptr);

public:
    template <typename T>
    any(const T& val) : m_data{ new T(val) }, copy(__copy<T>), destroy(__destroy<T>), get_type(__get_type<T>)
    {
    }

    any(const any& other);
    any(any&& other) noexcept;
    ~any();

    template <typename T>
    const any& operator=(const T& other)
    {
        this->reset();
        *this = any(other);

        return *this;
    }

    const any& operator=(const any& other);
    const any& operator=(any&& other) noexcept;

public:
    bool has_value() const noexcept;
    void reset() noexcept;
    void swap(any& other);
    const std::type_info& type() const;

private:
    void* m_data;
    void* (*copy)(void*);
    void (*destroy)(void*);
    const std::type_info& (*get_type)();
};

any::any(const any& other)
    : m_data{ other.copy(other.m_data) }, copy{ other.copy }, destroy{ other.destroy }, get_type{ other.get_type }
{
}

any::any(any&& other) noexcept
    : m_data{ other.m_data }, copy{ other.copy }, destroy{ other.destroy }, get_type{ other.get_type }
{
    other.m_data = nullptr;
}

any::~any()
{
    reset();
}

const any& any::operator=(const any& other)
{
    if (this != &other)
    {
        this->reset();

        m_data = other.copy(other.m_data);
        copy = other.copy;
        destroy = other.destroy;
        get_type = other.get_type;
    }

    return *this;
}

const any& any::operator=(any&& other) noexcept
{
    if (this != &other)
    {
        this->reset();

        m_data = other.m_data;
        other.m_data = nullptr;

        copy = other.copy;
        destroy = other.destroy;
        get_type = other.get_type;
    }
    return *this;
}

bool any::has_value() const noexcept
{
    return m_data != nullptr;
}

void any::reset() noexcept
{
    destroy(m_data);
    m_data = nullptr;
}

void any::swap(any& other)
{
    any tmp = std::move(other);
    other = std::move(*this);
    *this = std::move(tmp);
}

const std::type_info& any::type() const
{
    return get_type();
}

template <typename T>
const T& any_cast(const any& ob)
{
    if (typeid(T) != ob.type())
    {
        throw bad_any_cast();
    }

    return *static_cast<T*>(ob.m_data);
}

template <typename T>
const T* any_cast(const T* ptr)
{
    if (typeid(T) != ptr->type())
    {
        throw bad_any_cast();
    }

    return static_cast<T*>(ptr->m_data);
}

int main()
{
    try
    {
        any myAny = 42;
        std::cout << "Value of myAny: " << any_cast<int>(myAny) << std::endl;

        // Assign a double
        myAny = 0.25;
        std::cout << "Value of myAny after assignment: " << any_cast<double>(myAny) << std::endl;

        // Assign a string
        myAny = std::string("Hello, any!");
        std::cout << "Value of myAny after assignment: " << any_cast<std::string>(myAny) << std::endl;

        // Test with integer
        any myIntAny = 42;
        std::cout << "Value of myIntAny: " << any_cast<int>(myIntAny) << std::endl;

        // Test with string
        any myStringAny = std::string("Hello, any!");
        std::cout << "Value of myStringAny: " << any_cast<std::string>(myStringAny) << std::endl;

        // Test with swapping
        any anotherIntAny = 123;
        std::cout << "Value of anotherIntAny before swap: " << any_cast<int>(anotherIntAny) << std::endl;
        myIntAny.swap(anotherIntAny);
        std::cout << "Value of myIntAny after swap: " << any_cast<int>(myIntAny) << std::endl;
        std::cout << "Value of anotherIntAny after swap: " << any_cast<int>(anotherIntAny) << std::endl;

        // Test with bad_any_cast
        try
        {
            std::cout << any_cast<double>(myIntAny) << std::endl; // This should throw bad_any_cast
        }
        catch (const bad_any_cast& e)
        {
            std::cerr << "Caught exception: " << e.what() << std::endl;
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    std::any ob = 125;
    ob = "hello";
    std::cout << std::any_cast<const char*>(ob) << std::endl; // ok
    // std::cout << std::any_cast<int>(ob) << std::endl; // no!!!
}

// std::any ob = 125;
// ob = "hello";
// std::cout << std::any_cast<const char*>(ob) << std::endl; // ok
// // std::cout << std::any_cast<int>(ob) << std::endl; // no!!!
