#pragma once
#include "particle.hpp"
#include <vector>


namespace Simulation{
    struct Config{
        int n = 2;
        double h = 300;
        double w = 300;
        double r = 1;
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
        void set_piston_vel(double vel);
    
        double kinetic_energy() const;
        double temperature() const;
        
        double wall_impulse() const;
        void reset_wall_impulse();
    
        double current_time() const;
        const Config& config() const;
    
        const std::vector<Particle>& particles() const;

        double piston_x() const;
        double piston_imp() const;
        double piston_velocity() const;
    
    private:
        void move_particles();
        void collide_with_walls();
        void collide_pairs();
        void makeNetwork();
        void collide_cells(int i, int j);
        void clear_cells();
        void reflect_axis(double& pos, double& vel, double lo, double lo_u, double hi, double hi_u,double m, double r, double& impulse_lo, double& impulse_hi);
        void move_piston();
    
        Config config_;
        std::vector<Particle> particles_;
        double current_time_ = 0.0;

        int cols = 0;
        int rows = 0;
        double cell_w = 0;
        double cell_h = 0;
        std::vector<double> walls_imp;
        std::vector<std::vector<int>> cells;

        bool mode = false;

        //piston
        double piston_x_ = 0;
        double piston_max_x = 0;
        double piston_u = 0;
        double piston_imp_ = 0;

    };
};
