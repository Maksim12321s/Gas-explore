#include <gtest/gtest.h>
#include "simulation.hpp"
#include "species.hpp"
#include <cmath>
#include <iostream>
using namespace Simulation::units;
namespace {
    Particle make(double x, double y, double vx, double vy, double m = 1.0) {
        return Particle(1.0, m, Vec2(x, y), Vec2(vx, vy));
    }
    double countKineticEnergyProjection(double m, double v){
        return m*v*v/2;
    }
    void RunSteps(Simulation::Simulation& sim, int n){
        for(int i = 0; i < n; i++){
            sim.step();
        }
    }
    bool move_piston_to(Simulation::Simulation& sim, double target_x, double u){
        const double dir = (target_x > sim.piston_x()) ? 1 : -1;
        const int max_steps = 10'000'000;
        for(int i = 0; i < max_steps; ++i){
            const double x = sim.piston_x();
            if((dir > 0 && x >= target_x) || (dir < 0 && x <= target_x)) {
                sim.set_piston_vel(0);
                return true;
            }
            sim.set_piston_vel(dir*u);
            sim.step();
            if(sim.piston_x() == x){
                sim.set_piston_vel(0);
                return false;
            }
        }
        sim.set_piston_vel(0);
        return false;
    }
    double gas_area(const Simulation::Simulation& sim){
        const Simulation::Config& config = sim.config();
        return (config.w - sim.piston_x())*config.h;
    }


    class AdiabaticTest : public ::testing::Test {
        protected:
            void SetUp() override {
                conf = Simulation::make_config(Simulation::Argon, 300.0, 45 * nm, 45 * nm, 300);
            }

            void equilibrate(Simulation::Simulation& sim) {
                RunSteps(sim, 2000);
                T0 = sim.temperature();
                A0 = gas_area(sim);
            }
        
            Simulation::Config conf;
            double T0 = 0.0;
            double A0 = 0.0;
        };
}

TEST(Collision, HeadOnEqualMassesSwapVelocities) {
    Particle a = make(0.0, 0.0, 1.0, 0.0);
    Particle b = make(1.5, 0.0, -1.0, 0.0);

    ASSERT_TRUE(Simulation::resolve_collision(a, b));
    EXPECT_NEAR(a.vel.x, -1.0, 1e-12);
    EXPECT_NEAR(b.vel.x, 1.0, 1e-12);
}

TEST(Collision, HeadOnNotEqualMasses){
    Particle a = make(0.0, 0.0, 1.0, 0.0, 2.0);
    Particle b = make(1.5, 0.0, -1.0, 0.0, 1.0);

    ASSERT_TRUE(Simulation::resolve_collision(a,b));
    EXPECT_NEAR(a.vel.x, -1.0/3.0, 1e-12);
    EXPECT_NEAR(b.vel.x, 5.0/3.0, 1e-12);

}

TEST_F(AdiabaticTest, SlowCompressionKeepsTemperatureTimesAreaConstant) {
    Simulation::Simulation sim(conf);
    equilibrate(sim);
    const double u = conf.initial_speed/50.0;

    ASSERT_TRUE(move_piston_to(sim,0.4*conf.w,u)) << "поршень не дошел до цели";
    RunSteps(sim,2000);
    const double  T1 = sim.temperature();
    const double A1 = gas_area(sim);
    
    
    EXPECT_GT(T1, 1.4 * T0) << "газ должен заметно нагреться";
    EXPECT_NEAR((T1 * A1) / (T0 * A0), 1.0, 0.05);
}

TEST_F(AdiabaticTest, SlowCycleReturnsToInitialTemperature) {
    Simulation::Simulation sim(conf);
    equilibrate(sim);
    const double u = conf.initial_speed/50.0;

    ASSERT_TRUE(move_piston_to(sim,0.4*conf.w,u)) << "поршень не дошел до цели";
    RunSteps(sim,2000);
    ASSERT_TRUE(move_piston_to(sim,0.0,u)) << "поршень не дошел до цели";
    RunSteps(sim,2000);

    const double  T1 = sim.temperature();

    EXPECT_NEAR( T1 / T0, 1.0, 0.05);
}

TEST_F(AdiabaticTest, FastCompressionHeatsMoreThanAdiabat) {
    Simulation::Simulation sim(conf);
    equilibrate(sim);

    const double u = conf.initial_speed;   
    ASSERT_TRUE(move_piston_to(sim, 0.4 * conf.w, u));
    RunSteps(sim, 2000);

    const double T1 = sim.temperature();
    const double A1 = gas_area(sim);


    EXPECT_GT((T1 * A1) / (T0 * A0), 1.05) << "быстрое сжатие должно нагревать сильнее адиабаты";
}

