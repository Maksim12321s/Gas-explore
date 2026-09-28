#include <SFML/Graphics.hpp>
#include "include/simulation.hpp"
#include <vector>
#include <iostream>
#include <sstream>
using namespace Simulation;


void makeColor(sf::CircleShape& c, float en, Particle p){
    double sEn = p.m * (p.vel.x * p.vel.x + p.vel.y * p.vel.y) / 2;
    double k = sEn/en;
    c.setFillColor(sf::Color(100,k*255,100));
}

void refreshLabel(sf::Text& label, Simulation::Simulation sim){
    Config c = sim.config();
    std::ostringstream text;
    text << "E = " << sim.kinetic_energy()
        << "   kT = " << sim.temperature()
        << "   dt   = " << c.dt 
        << "   time = " << sim.current_time() ;
    label.setString(text.str());
}

int main(){
    bool wasPressed = false;
    Config conf;
    conf.r = 10;
    conf.h = 800;
    conf.w = 800;
    conf.n = 100;

    
    Simulation::Simulation sim(conf);
    const std::vector<Particle>& particles = sim.particles();


    std::vector<sf::CircleShape> circles;
    for(int i = 0; i < particles.size(); i++){
        sf::CircleShape circle(conf.r);
        circle.setFillColor(sf::Color::Cyan);   
        circle.setOrigin(conf.r, conf.r);
        circle.setPosition(particles[i].pos.x,particles[i].pos.y);
        circles.push_back(circle);
    }

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        return 1;  // шрифт не найден
    }

    sf::Text label;
    label.setFont(font);
    label.setCharacterSize(18);              
    label.setFillColor(sf::Color::White);
    label.setPosition(10.f, 10.f);           

    
    sf::RenderWindow window(sf::VideoMode(conf.h,conf.h), "Gas sim");


    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Escape){
                    wasPressed = !wasPressed;
                }else if(event.key.code == sf::Keyboard::Up){
                    sim.addDT(0.005);
                }else if(event.key.code == sf::Keyboard::Down){
                    sim.addDT(-0.005);
                }
            }
        }

        window.clear(sf::Color::Black);
        for(int i = 0; i < particles.size(); i++){
            makeColor(circles[i],sim.kinetic_energy()/conf.n, particles[i]);
            circles[i].setPosition(particles[i].pos.x,particles[i].pos.y);
            window.draw(circles[i]);
        }
        refreshLabel(label, sim);
        window.draw(label);
        window.display();
        if(!wasPressed) sim.step();
    }


    return 0;
}