#pragma once
#include "particle.hpp"
#include <vector>


namespace Simulation{
    struct Config{
        int n = 300;
        double h = 300;
        double w = 300;
        double r = 0.8;
        double m = 1;
        double dt = 0.05;
        double initial_speed = 1;
        unsigned seed = 42;
    };
    
    bool resolve_collision(Particle& a, Particle& b);
    
    class Simulation{
    public:
        explicit Simulation(const Config& config);
        void step();
        void heat(const vec2::Vec2& center, double radius, double energy_factor);
    
        double kinetic_energy() const;
        double temperature() const;
    
        double wall_impulse() const;
        void reset_wall_impulse();
    
        double current_time() const;
        const Config& config() const;
    
        const std::vector<Particle>& particles() const;
    
    private:
        void move_particles();
        void collide_with_walls();
        void collide_pairs();
    
        Config config_;
        std::vector<Particle> particles_;
        double current_time_ = 0.0;
        double wall_impulse_ = 0.0;

    
    
    };
};
