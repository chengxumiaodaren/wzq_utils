#include <iostream>
#include <string>
#include <type_traits>

// checking if Foo is constructible from double will cause a hard error
struct Foo {
    template <class T>
    struct sfinae_unfriendly_check {
        static_assert(!std::is_same_v<T, double>);
    };

    template <class T>
    Foo(T, sfinae_unfriendly_check<T> = {});
};

template <class... Ts>
struct first_constructible {
    template <class T, class... Args>
    struct is_constructible_x : std::is_constructible<T, Args...> {
        using type = T;
    };
    struct fallback {
        static constexpr bool value = true;
        using type = void;  // type to return if nothing is found
    };

    template <class... Args>
    using with = typename std::disjunction<is_constructible_x<Ts, Args...>..., fallback>::type;
};

// OK, is_constructible<Foo, double> not instantiated
static_assert(std::is_same_v<first_constructible<std::string, int, Foo>::with<double>, int>);

static_assert(std::is_same_v<first_constructible<std::string, int>::with<>, std::string>);
static_assert(std::is_same_v<first_constructible<std::string, int>::with<const char*>, std::string>);
static_assert(std::is_same_v<first_constructible<std::string, int>::with<void*>, void>);

// func is enabled if all Ts... have the same type as T
template <typename T, typename... Ts>
std::enable_if_t<std::conjunction_v<std::is_same<T, Ts>...>> func(T, Ts...) {
    std::cout << "all types in pack are T\n";
}

// otherwise
template <typename T, typename... Ts>
std::enable_if_t<!std::conjunction_v<std::is_same<T, Ts>...>> func(T, Ts...) {
    std::cout << "not all types in pack are T\n";
}

static_assert(std::is_same<std::bool_constant<false>, typename std::negation<std::bool_constant<true>>::type>::value,
              "");
static_assert(std::is_same<std::bool_constant<true>, typename std::negation<std::bool_constant<false>>::type>::value,
              "");


int main() {
    func(1, 2, 3);
    func(1, 2, "hello!");
    std::cout << std::boolalpha;
    std::cout << std::negation<std::bool_constant<true>>::value << '\n';
    std::cout << std::negation<std::bool_constant<false>>::value << '\n';
    return 0;
}