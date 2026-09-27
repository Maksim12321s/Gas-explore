#pragma once
#include "vec2.hpp"
using namespace vec2;
#include <iostream>
struct Particle{
    double r;
    double m;
    Vec2 vel;
    Vec2 pos;
    Particle():Particle(0,0,Vec2(), Vec2()){};
    Particle(double r, double m, Vec2 pos, Vec2 velocity): r(r), m(m), pos(pos), vel(velocity){};
    void move(double dt){
        pos = pos + vel*dt;
    };
    void Status(){
        std::cout << "position: (" << pos.x << ", " << pos.y << ")" << std::endl;
        std::cout << "velocity: (" << vel.x << ", " << vel.y << ")" << std::endl;  
    };
    Vec2 Getpos() const{
        return pos;
    };
};
