#include "renderer.hpp"
#include <sstream>
#include <vector>

unsigned Renderer::window_height() const { return height_; }
unsigned Renderer::window_width() const { return width_; }

Renderer::Renderer(const Simulation::Config& config, double pixels_per_meter) : scale_(pixels_per_meter),
    height_(static_cast<unsigned>(config.h * pixels_per_meter)),
    width_(static_cast<unsigned>(config.w * pixels_per_meter))    
{

    const std::string font_path = "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"; 
    if (!font_.loadFromFile(font_path)) {
        throw std::runtime_error("Не удалось найти шрифт" + font_path);
    }

    label_.setFont(font_);
    label_.setCharacterSize(18);              
    label_.setFillColor(sf::Color::White);
    label_.setPosition(10.f, 10.f);  


}

sf::Vector2f Renderer::to_screen(const vec2::Vec2& pos){
    return sf::Vector2f(static_cast<float>(pos.x * scale_),
                        static_cast<float>(pos.y * scale_));
}

void Renderer::update_label(const Simulation::Simulation& sim, int steps){
    std::ostringstream text;
    text << "kT = " << sim.temperature()
         << "   steps = " << steps
         << "   time = " << sim.current_time();
    label_.setString(text.str());
}

sf::Color Renderer::particle_color(const Particle& p, double mean_energy){
    double sEn = p.m * (p.vel.x * p.vel.x + p.vel.y * p.vel.y) / 2;
    double k = sEn/mean_energy;
    double t = std::min(k/3.0, 1.0);
    auto g = static_cast<sf::Uint8>(255 * t);
    return sf::Color(255*t,0,(1-t)*255);

}

void Renderer::draw(sf::RenderWindow& w, const Simulation::Simulation& sim, int steps){
    const std::vector<Particle>& p = sim.particles();
    double sE = sim.kinetic_energy() / p.size();
    for(int i = 0; i < p.size(); i++){
        circle_.setFillColor(particle_color(p[i],sE));
        circle_.setRadius(p[i].r*scale_);
        circle_.setOrigin(p[i].r * scale_,p[i].r * scale_);
        circle_.setPosition(to_screen(p[i].pos));
        w.draw(circle_);
    }
    update_label(sim, steps);
    w.draw(label_);

};