#include "statistics.hpp"
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stdexcept>

namespace analysis{
    Sample::Sample(double t, double ke, double temp, double a, double pw, double p) : time(t),
                                                                                      kinetic_energy(ke),
                                                                                      temperature(temp), 
                                                                                      area(a), 
                                                                                      piston_work(pw), 
                                                                                      pressure(p)
    {


    }
    void TimeSeries::record(const Simulation::Simulation& sim){
        samples_.push_back(Sample(sim.current_time(),sim.kinetic_energy(),sim.temperature(),sim.area(), 0.0, 0.0));
        if(samples_.size() > 1'000'000){
            write_csv(Path);
            samples_.clear();
        }
        last_time_ = sim.current_time();
        last_wall_impulse_ = sim.wall_impulse();
    }

    void TimeSeries::write_csv(const std::string& path) const{
        std::ofstream out(path);
        if (!out) {
            throw std::runtime_error("Cannot open file for writing: " + path);
        }
    
        out << std::setprecision(9);
        out << "time,kinetic_energy,temperature,area,piston_work,pressure\n";
    
        for (const auto& s : samples_) {
            out << s.time << ','
                << s.kinetic_energy << ','
                << s.temperature << ','
                << s.area << ','
                << s.piston_work << ','
                << s.pressure << '\n';
        }
    };

    SpeedHistogramm::SpeedHistogramm(double max_speed, int bins) : max_speed_(max_speed){
        counts_ = std::vector<long long>(bins, 0);
    }
    void SpeedHistogramm::add(const Simulation::Simulation& sim){
        const std::vector<Particle>& p = sim.particles();
        double vel = 0.0;
        int ind = 0;
        int bins = counts_.size();
        for(int i = 0; i < p.size(); i++){
            vel = p[i].vel.len();
            ind = static_cast<int>(vel/max_speed_*bins);
            if (ind < 0 || ind >= bins) { 
                // ++overflow_;
                continue;
            }
            ++counts_[ind];
            ++total_;
        }
    }
    void SpeedHistogramm::write_csv(const std::string& Path) const{
        std::ofstream out(Path);
        
        for(int i = 0; i < counts_.size(); i++){
            out << i;
            if(i != counts_.size() - 1){
                out << ",";
            }else{
                out << "\n";
            }
        }
        for(int i = 0; i < counts_.size(); i++){
            out << counts_[i] << ",";
        }

    }




};