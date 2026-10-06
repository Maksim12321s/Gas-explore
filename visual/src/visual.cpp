#include <SFML/Graphics.hpp>
#include "simulation.hpp"
#include "renderer.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include "species.hpp"
using namespace Simulation;
using namespace Simulation::units;




int main(){
    bool wasPressed = false;
    const bool debug = true;
    int steps = 5;

    Simulation::Config conf = Simulation::make_config(Simulation::Argon, 300.0, 20*nm, 20*nm, 200);
    
    const double u = 0.01;

    const double scale = 800/conf.w;
    
    Simulation::Simulation sim(conf);

    Renderer r(conf,scale);
    
    sf::RenderWindow window(sf::VideoMode(r.window_width(),r.window_height()), "Gas sim");
    window.setFramerateLimit(60);


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
                    steps += 1;
                }else if(event.key.code == sf::Keyboard::Down){
                    steps -= 1;
                } 
                
                if(debug == true){

                }
            }
            // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            //     sim.set_piston_vel(u);
            // }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            //     sim.set_piston_vel(-u);
            // }else{
            //     sim.set_piston_vel(0);
            // }
        }

        window.clear(sf::Color::Black);
        r.draw(window,sim,steps);
        window.display();
        if(!wasPressed) {
            for(int i = 0; i < steps; i++){
                sim.step();
            }
        }
    }


    return 0;
}