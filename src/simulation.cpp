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



    Simulation::Simulation(const Config& config) : config_(config) {
        const int n = config_.n;
        if (n <= 0) {
            throw std::invalid_argument("number of particles must be positive");
        }


        const int cols = static_cast<int>(std::ceil(std::sqrt(static_cast<double>(n*config_.w/config_.h))));
        const int rows = static_cast<int>(std::ceil(std::sqrt(static_cast<double>(n*config_.h/config_.w))));
        const double spacing_x = config_.w / cols;
        const double spacing_y = config_.h / rows;


        if (spacing_x <= 2.0 * config_.r || spacing_y <= 2.0 * config_.r  ) {
            throw std::invalid_argument("particles do not fit into the box");
        }
        std::mt19937 rng(config_.seed);
        std::uniform_real_distribution<double> angle_dist(0.0, 2.0 * M_PI);
        for(int i = 0; i < config_.n; i++){
            const int col = i % cols;  
            const int row = i / rows;

            double phi = angle_dist(rng);
            Vec2 V = Vec2(cos(phi),sin(phi)) * config_.initial_speed;
            Vec2 pos = vec2::Vec2((col + 0.5) * spacing_x, (row + 0.5) * spacing_y);
            particles_.push_back(Particle(config_.r,config_.m,pos,V));

        }
        this->cols = config_.w / (2*config_.r);
        this->rows = config_.h / (2*config_.r);
        cell_h = config_.h / this->rows;
        cell_w = config_.w / this->cols;

        cells.resize(this->cols* this->rows);
        walls_imp.resize(4);

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
            
            // if((p.pos.x + p.r > config_.w && p.vel.x > 0) || (p.pos.x - p.r < 0 && p.vel.x < 0)){
            //     p.vel.x = p.vel.x * -1;
            //     wall_impulse_ += 2 * p.m * std::abs(p.vel.x);
            // }
            // if((p.pos.y + p.r > config_.h && p.vel.y > 0) || (p.pos.y - p.r < 0 && p.vel.y < 0) ){
            //     p.vel.y = p.vel.y * -1;
            //     wall_impulse_ += 2 * p.m * std::abs(p.vel.y);
            // }
            reflect_axis(p.pos.x,p.vel.x,0,config_.w,p.m,p.r,walls_imp[0],walls_imp[1]);
            reflect_axis(p.pos.y,p.vel.y,0,config_.h,p.m,p.r,walls_imp[2],walls_imp[2]);
        }
    }
    void Simulation::collide_cells(int a, int b){
        if(a != b){
            for(int i = 0; i < cells[a].size(); i++){
                for(int j = 0; j < cells[b].size(); j++){
                    resolve_collision(particles_[cells[a][i]],particles_[cells[b][j]]);
                }
            }
        }else{
            for(int i =0; i < cells[a].size(); i++){
                for(int j =  i + 1; j < cells[a].size(); j++){
                    resolve_collision(particles_[cells[a][i]], particles_[cells[a][j]]);
                }
            }
        }
        
    }
    
    void Simulation::clear_cells(){
        for(int i = 0; i < cells.size(); i++){
            cells[i].clear();
        }
    }
    void Simulation::collide_pairs() {
           
            clear_cells();
            makeNetwork();
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    if(i == 0 && j != cols - 1){
                        collide_cells(i*cols + j, i*cols + j + 1);
                    }else if(i != 0 && j == cols - 1){
                        collide_cells(i*cols + j, (i-1)*cols + j);
                        collide_cells(i*cols + j, (i-1)*cols + j - 1);
                    }else if(i != 0 && j == 0){
                        collide_cells(i*cols + j, (i-1)*cols + j);
                        collide_cells(i*cols + j, (i-1)*cols + j + 1);
                        collide_cells(i*cols + j, i*cols + j + 1);
                    }else if(i ==0 && j == cols - 1) {
    
                    }else{
                        collide_cells(i*cols + j, (i-1)*cols + j - 1);
                        collide_cells(i*cols + j, (i-1)*cols + j);
                        collide_cells(i*cols + j, (i-1)*cols + j + 1);
                        collide_cells(i*cols + j, i*cols + j + 1);
                    }
                    collide_cells(i*cols + j, i*cols + j);
                }
            }
        
        
        
    }

    void Simulation::makeNetwork(){
        for(int i = 0; i < particles_.size(); i++){
            int r = (int)(particles_[i].pos.y / cell_h);
            int c = (int)(particles_[i].pos.x / cell_w);
            if(r < 0) r = 0;
            if(c < 0) c = 0;
            if(r >= rows) r = rows - 1;
            if(c >= cols) c = cols - 1;
            cells[r*cols + c].push_back(i);
        }
    }

    
    void Simulation::reflect_axis(double& pos, double& vel, double lo, double hi,double m, double r, double& impulse_lo, double& impulse_hi){
        if(pos - r < lo){
            pos = 2*(lo + r) - pos;
            if(vel < 0){
                vel = -vel;
                impulse_lo += 2 * m * vel;
            }
        }else if(pos + r > hi){
            pos = 2*(hi - r) - pos;
            if(vel > 0){
                vel = -vel;
                impulse_hi += 2.0*m*(-vel);
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
