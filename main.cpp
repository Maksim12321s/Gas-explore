#include <cmath>
#include <iostream>

#include "simulation.hpp"

int main() {

    Simulation::Config config;
    config.n = 10;
    config.w = 100.0;
    config.h = 100.0;
    config.r = 0.8;
    config.m = 1.0;
    config.initial_speed = 1.0;
    config.dt = 0.05;
    config.seed = 42;

    Simulation::Simulation sim(config);

    const double e0 = sim.kinetic_energy();
    const double e_expected =
        config.n * 0.5 * config.m * config.initial_speed * config.initial_speed;
    std::cout << "Initial energy: " << e0 << "  (expected " << e_expected << ")\n";

    std::cout << "First particles:\n";
    for (int i = 0; i < 3; ++i) {
        const auto& p = sim.particles()[i];
        std::cout << "  pos = (" << p.pos.x << ", " << p.pos.y << ")"
                  << "  vel = (" << p.vel.x << ", " << p.vel.y << ")"
                  << "  |v| = " << p.vel.len() << '\n';
    }


    const int total_steps = 6000;
    const int warmup_steps = 2000;  
    double max_drift = 0.0;

    for (int s = 1; s <= total_steps; ++s) {
        sim.step();
        if (s == warmup_steps) {
            sim.reset_wall_impulse();
        }

        const double drift = std::abs(sim.kinetic_energy() - e0) / e0;
        if (drift > max_drift) max_drift = drift;

        if (s % 1000 == 0) {
            std::cout << "step " << s << "  t = " << sim.current_time()
                      << "  E = " << sim.kinetic_energy() << '\n';
        }
    }
    std::cout << "Max relative energy drift: " << max_drift << '\n';


    int outside = 0;
    for (const auto& p : sim.particles()) {
        if (p.pos.x < 0 || p.pos.x > config.w || p.pos.y < 0 || p.pos.y > config.h) {
            ++outside;
        }
    }
    std::cout << "Particles outside the box: " << outside << '\n';

    const double measure_time = (total_steps - warmup_steps) * config.dt;
    const double perimeter = 2.0 * (config.w + config.h);
    const double p_measured = sim.wall_impulse() / (measure_time * perimeter);
    const double p_ideal = config.n * sim.temperature() / (config.w * config.h);
    std::cout << "Temperature kT: " << sim.temperature() << '\n'
              << "Pressure: simulation = " << p_measured << ", ideal gas = " << p_ideal
              << ", ratio = " << p_measured / p_ideal << '\n';

    const bool ok = max_drift < 1e-9 && outside == 0;
    std::cout << (ok ? "OK\n" : "FAILED\n");
    return ok ? 0 : 1;
}