#pragma once
#include <string>
#include <vector>
#include "simulation.hpp"

namespace analysis{

    struct Sample{
        double time;
        double kinetic_energy;
        double temperature;
        double area;
        double piston_work;
        double pressure;
        Sample(double t, double ke, double temp, double a, double pw, double p);
    };
    
    class TimeSeries{
        public:
            void record(const Simulation::Simulation& sim);
            const std::vector<Sample>& samples() const;
            void write_csv(const std::string& path = "output/TimeStats.csv") const;

        private:
            const std::string Path = "output/TimeStats.csv";
            std::vector<Sample> samples_;
            double last_wall_impulse_ = 0.0;
            double last_time_ = 0.0;
    };

    class SpeedHistogramm {
        public:
            SpeedHistogramm(double max_speed, int bins);
            void add(const Simulation::Simulation& sim);
            std::vector<double> density() const;
            void write_csv(const std::string& path = "output/Histogramm.csv") const;

        private:
            double max_speed_ = 0.0;
            std::vector<long long> counts_;
            long long total_ = 0.0;
    };
};
