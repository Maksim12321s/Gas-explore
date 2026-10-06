#pragma once

#include <SFML/Graphics.hpp>
#include "simulation.hpp"
#include "particle.hpp"

class Renderer{
    public:
        Renderer(const Simulation::Config& config, double pixels_per_meter);

        void draw(sf::RenderWindow& w,const Simulation::Simulation& sim, int steps);
        

        unsigned window_width() const;
        unsigned window_height() const;
    
        private:
            sf::Vector2f to_screen(const vec2::Vec2& pos);
            sf::Color particle_color(const Particle& p, double mean_energy);


            void update_label(const Simulation::Simulation& sim, int steps);

            double scale_;
            unsigned width_;
            unsigned height_;
            sf::CircleShape circle_;      // один круг для всех частиц
            sf::RectangleShape piston_;
            sf::Font font_;
            sf::Text label_;

};