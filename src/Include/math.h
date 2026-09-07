// Copyright (c) 2026 jozin1224
namespace std {
    constexpr double PI = 3.14159265358979323846;
    template<typename T>
    constexpr T abs(T x) {
        return (x < 0) ? -x : x;
    }
    double sqrt(double x) {
        if (x < 0) return 0.0;
        if (x == 0.0 || x == 1.0) return x;
        double res = x / 2.0; 
        double last;
        do {
            last = res;
            res = (res + x / res) / 2.0;
        } while (abs(res - last) > 1e-15);

        return res;
    }
    double pow(double base, int exp) {
        if (exp == 0) return 1.0;
        if (exp < 0) {
            base = 1.0 / base;
            exp = -exp;
        }
        
        double res = 1.0;
        while (exp > 0) {
            if (exp & 1) res *= base;
            base *= base;
            exp >>= 1;
        }
        return res;
    }
    static double fmod_pi(double x) {
        while (x > PI)  x -= 2.0 * PI;
        while (x < -PI) x += 2.0 * PI;
        return x;
    }

    double sin(double x) {
        x = fmod_pi(x);
        double x2 = x * x;
        double termo1 = x;
        double termo3 = (termo1 * x2) / 6.0;
        double termo5 = (termo3 * x2) / 20.0;
        double termo7 = (termo5 * x2) / 42.0;
        
        return termo1 - termo3 + termo5 - termo7;
    }

    
    double cos(double x) {
        
        return sin(x + (PI / 2.0));
    }
}

using std::abs;
using std::cos;
using std::sin;
using std::sqrt;
using std::fmod_pi;
using std::pow;
