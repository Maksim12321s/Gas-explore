#pragma once
#include <math.h>
namespace vec2{
    struct Vec2{
        double x;
        double y;
        Vec2():Vec2(0,0){};
        Vec2(double x, double y): x(x), y(y){};
        inline double len() const{
            return sqrt(x*x + y*y);
        }
        inline void norm(){
            double l = len();
            x/= l;
            y/=l;
        }
    };
    inline Vec2 operator+(Vec2 a, Vec2 b){
        return Vec2(a.x + b.x, a.y + b.y);
    };
    inline Vec2 operator-(Vec2 a, Vec2 b){
        return Vec2(a.x-b.x, a.y-b.y);
    };
    inline Vec2 operator*(Vec2 a, double k){
        return Vec2(a.x*k, a.y*k);
    }
    inline Vec2 operator/(Vec2 a, double k){
        return Vec2(a.x/k, a.y/k);
    }
    
    inline double operator*(Vec2 a, Vec2 b){
        return a.x*b.x + a.y*b.y;
    }
    
};



