#include <iostream>

template <int M, int K, int S>
struct Unit {
    enum { metre = M, kilogram = K, second = S };
};


template <typename U>
class Value {
   private:
    long double magnitude_{0.0};

   public:
    constexpr explicit Value(const long double magnitude) : magnitude_(magnitude) {}

    constexpr long double GetMagnitude() const noexcept { return magnitude_; }
};

using Length = Value<Unit<0, 0, 1>>;
using Distance = Value<Unit<0, 1, 0>>;
using SpeedMs = Value<Unit<0, 1, 1>>;
using SpeedKmh = Value<Unit<1, 1, 0>>;
using Time = Value<Unit<1, 0, 0>>;

// 通过使用这种方式可以降低沟通成本，一个speed，如果是int，单位可能是m/s，也有可能是km/h，
// 需要沟通传入的究竟是什么单位，如果沟通有误就会引起严重后果，
// 都统一使用这种方式就肯定会保证使用的是同一个单位，只能传入SpeedMs或SpeedKmh
void UseSpeed(const SpeedMs& speed) { std::cout << "speed " << speed.GetMagnitude() << std::endl; }

template <int M, int K, int S>
constexpr Value<Unit<M, K, S>> operator+(const Value<Unit<M, K, S>>& l, const Value<Unit<M, K, S>>& r) noexcept {
    return Value<Unit<M, K, S>>(l.GetMagnitude() + r.GetMagnitude());
}

// 这里的M，K，S的取值是个学问，可以自己根据实际情况计算好，distance=time*speed
// 每个都是不同的M，K，S，当我们定义他们的加减乘除操作符的时候就需要考虑好，
// 如果能够保证time和speed的M，K，S运算后是distance的类型呢
template <int M1, int K1, int S1, int M2, int K2, int S2>
constexpr Value<Unit<M1 + M2, K1 + K2, S1 + S2>> operator*(const Value<Unit<M1, K1, S1>>& l,
                                                           const Value<Unit<M2, K2, S2>>& r) noexcept {
    return Value<Unit<M1 + M2, K1 + K2, S1 + S2>>(l.GetMagnitude() * r.GetMagnitude());
}

void TestValue() {
    SpeedMs ms{1.2};
    UseSpeed(ms);
    SpeedMs mss = SpeedMs{1.4} + SpeedMs{1.5};
    UseSpeed(mss);
}

// 为了更加直观的看也可以为文字添加自定义后缀，所谓的文字操作符
constexpr SpeedMs operator"" _ms(long double magnitude) {
    return SpeedMs(magnitude);
}

constexpr SpeedKmh operator"" _kmh(long double magnitude) {
    return SpeedKmh(magnitude);
}

constexpr Length operator"" _m(long double magnitude) {
    return Length(magnitude);
}

void TestPostfix() {
    constexpr SpeedMs speed_ms = 1.2_ms;
    constexpr SpeedKmh speed_kmh = 1.3_kmh;
    constexpr Length length = 1.4_m;
    std::cout << speed_ms.GetMagnitude() << std::endl;
    std::cout << speed_kmh.GetMagnitude() << std::endl;
    std::cout << length.GetMagnitude() << std::endl;

    long l;
    double d;
    long double ld;
    std::cout << sizeof(l) << std::endl;
    std::cout << sizeof(d) << std::endl;
    std::cout << sizeof(ld) << std::endl;
}

int main() {
    TestValue();
    TestPostfix();
    return 0;
}