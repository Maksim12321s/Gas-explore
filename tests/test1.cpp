#include <gtest/gtest.h>
#include "simulation.hpp"

namespace {
    // Частица радиуса 1 — вспомогательная функция, чтобы тесты были короче.
    Particle make(double x, double y, double vx, double vy, double m = 1.0) {
        return Particle(1.0, m, Vec2(x, y), Vec2(vx, vy));
    }
    Simulation::Config Set_up(double r, double m, double n, double h, double w, double dt, double initial_speed = 1, double seed = 42){

    }
}

TEST(Collision, HeadOnEqualMassesSwapVelocities) {
    Particle a = make(0.0, 0.0, 1.0, 0.0);
    Particle b = make(1.5, 0.0, -1.0, 0.0);

    ASSERT_TRUE(Simulation::resolve_collision(a, b));
    EXPECT_NEAR(a.vel.x, -1.0, 1e-12);
    EXPECT_NEAR(b.vel.x, 1.0, 1e-12);
}

TEST(Simulation, VariableMasses) {
    
}