#pragma once
#include <string>
#include <cmath>
#include "simulation.hpp"



namespace Simulation{
    
    namespace units{
        inline constexpr double k_B = 1.380649e-23;       // постоянная Больцмана, Дж/К
        inline constexpr double amu = 1.66053906660e-27;  // атомная единица массы, кг
        inline constexpr double nm  = 1e-9;              // нанометр, м
    };

    struct Species{
        std::string_view name;
        double mass;
        double radius;
    };

    inline constexpr Species Helium {"He", 4.0026 * units::amu, 0.130 * units::nm};
    inline constexpr Species Neon {"Ne", 20.180 * units::amu, 0.138 * units::nm};
    inline constexpr Species Argon {"Ar", 39.948  * units::amu, 0.170 * units::nm};
    inline constexpr Species Krypton {"Kr",  83.798  * units::amu, 0.180 * units::nm};
    inline constexpr Species Xenon {"Xe", 131.29   * units::amu, 0.198 * units::nm};

    inline Config make_config(const Species& s, double temperature,
        double box_w, double box_h, int n) {
        Config c;
        c.m = s.mass;
        c.r = s.radius;
        c.w = box_w;
        c.h = box_h;
        c.n = n;
        c.initial_speed = std::sqrt(2.0 * units::k_B * temperature / s.mass);

        c.dt = 0.5 * s.radius / (3.0 * c.initial_speed);
        return c;
    }
};

