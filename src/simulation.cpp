#include "simulation.hpp"
#include <random>
#include <cmath>
namespace Simulation {

    
    bool resolve_collision(Particle& a, Particle& b) {
        Vec2 n = (b.pos - a.pos);
        double s = n.len();
        if(s < a.r + b.r && s > 0){
            n.norm();
            double vn = (a.vel - b.vel) * n;
            if(vn > 0){
                double J = 2* a.m * b.m / (a.m + b.m) * vn;
                a.vel = a.vel - n * (J/a.m);
                b.vel = b.vel + n * (J/b.m);
                return true;
            }
            
        }
        return false;
    }
    
    void Simulation::heat(const vec2::Vec2& center, double radius, double energy_factor) {
        const double speed_factor = std::sqrt(energy_factor);
        const double max_speed = 0.5 * config_.r / config_.dt;
    
        for (auto& p : particles_) {
            if ((p.pos - center).len() < radius) {
                p.vel = p.vel * speed_factor;
                const double speed = p.vel.len();
                if (speed > max_speed) {
                    p.vel = p.vel * (max_speed / speed);
                }
            }
        }
    }

    void Simulation::addDT(double dt){
        if (config_.dt <= 0){
            config_.dt = 0.05;
        }
        config_.dt += dt;
        
    }


    Simulation::Simulation(const Config& config) : config_(config) {
        const int n = config_.n;
        if (n <= 0) {
            throw std::invalid_argument("number of particles must be positive");
        }


        const int cols = static_cast<int>(std::ceil(std::sqrt(static_cast<double>(n))));
        const double spacing = config_.w / cols;


        if (spacing <= 2.0 * config_.r) {
            throw std::invalid_argument("particles do not fit into the box");
        }
        std::mt19937 rng(config_.seed);
        std::uniform_real_distribution<double> angle_dist(0.0, 2.0 * M_PI);
        for(int i = 0; i < config_.n; i++){
            const int col = i % cols;  
            const int row = i / cols;

            double phi = angle_dist(rng);
            Vec2 V = Vec2(cos(phi),sin(phi)) * config_.initial_speed;
            Vec2 pos = vec2::Vec2((col + 0.5) * spacing, (row + 0.5) * spacing);
            particles_.push_back(Particle(config_.r,config_.m,pos,V));

        }


    }
    
    void Simulation::step() {
        move_particles();
        collide_with_walls();
        collide_pairs();
        current_time_ += config_.dt;
    }
    
    void Simulation::move_particles() {
        for(int i = 0; i < particles_.size(); i++){
            particles_[i].pos = particles_[i].pos +  particles_[i].vel * config_.dt;
        }
    }
    
    void Simulation::collide_with_walls() {

        for(int i = 0; i < particles_.size(); i++){
            Particle& p = particles_[i];
            
            if((p.pos.x + p.r > config_.w && p.vel.x > 0) || (p.pos.x - p.r < 0 && p.vel.x < 0)){
                p.vel.x = p.vel.x * -1;
                wall_impulse_ += 2 * p.m * std::abs(p.vel.x);
            } 
            if((p.pos.y + p.r > config_.h && p.vel.y > 0) || (p.pos.y - p.r < 0 && p.vel.y < 0) ){
                p.vel.y = p.vel.y * -1;
                wall_impulse_ += 2 * p.m * std::abs(p.vel.y);
            }
        }
    }
    
    void Simulation::collide_pairs() {

        for(int i = 0; i < particles_.size(); i++){
            for(int j = i+1; j < particles_.size(); j++){
                resolve_collision(particles_[i], particles_[j]);
            }
        }
    }
    
    double Simulation::kinetic_energy() const {
        double e = 0;
        for(int i = 0; i < particles_.size(); i++){
            e += particles_[i].m * (pow(particles_[i].vel.x,2) + pow(particles_[i].vel.y,2))/2;
        }
        return e;
    }
    
    double Simulation::temperature() const {
        if (particles_.empty()) return 0.0;
        return kinetic_energy() / particles_.size();
    }
    
    double Simulation::wall_impulse() const { return wall_impulse_; }
    void Simulation::reset_wall_impulse() { wall_impulse_ = 0.0; }
    double Simulation::current_time() const { return current_time_; }
    const Config& Simulation::config() const { return config_; }
    const std::vector<Particle>& Simulation::particles() const { return particles_; }


};