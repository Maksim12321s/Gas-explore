// #include <SFML/Graphics.hpp>
// #include "simulation.hpp"
// #include <vector>
// #include <iostream>
// #include <sstream>
// #include "species.hpp"
// using namespace Simulation;
// using namespace Simulation::units;


// void makeColor(sf::CircleShape& c, float en, const Particle& p){
//     double sEn = p.m * (p.vel.x * p.vel.x + p.vel.y * p.vel.y) / 2;
//     double k = sEn/en;
//     double t = std::min(k/3.0, 1.0);
//     auto g = static_cast<sf::Uint8>(255 * t);
//     c.setFillColor(sf::Color(255*t,0,(1-t)*255));
// }
// void refreshLabel(sf::Text& label, const Simulation::Simulation& sim, int steps){
//     Config c = sim.config();
//     std::ostringstream text;
//     text<< "E = " << sim.kinetic_energy()
//         << "   kT = " << sim.temperature()
//         << "   steps   = " << steps
//         << "   piston vel = " << sim.piston_velocity() 
//         << "   time = " << sim.current_time() ;
//     label.setString(text.str());
// }

// int main(){
//     bool wasPressed = false;
//     bool debug = true;
    

//     Simulation::Config conf = Simulation::make_config(Simulation::Argon, 300.0, 20*nm, 20*nm, 200);
//     int steps = 5;
//     const double u = 0.01;

//     const double scale = 800/conf.w;
    
//     Simulation::Simulation sim(conf);
//     const std::vector<Particle>& particles = sim.particles();


//     std::vector<sf::CircleShape> circles;
//     for(int i = 0; i < particles.size(); i++){
//         sf::CircleShape circle(conf.r * scale);
//         circle.setFillColor(sf::Color::Cyan);   
//         circle.setOrigin(conf.r * scale, conf.r*scale);
//         circle.setPosition(particles[i].pos.x*scale,particles[i].pos.y*scale);
//         circles.push_back(circle);
//     }

//     sf::RectangleShape piston;
//     piston.setFillColor(sf::Color(90, 90, 110));
//     piston.setPosition(0.f, 0.f);
//     sf::Font font;
//     if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
//         return 1;  // шрифт не найден
//     }

//     sf::Text label;
//     label.setFont(font);
//     label.setCharacterSize(18);              
//     label.setFillColor(sf::Color::White);
//     label.setPosition(10.f, 10.f);           

    
//     sf::RenderWindow window(sf::VideoMode(conf.w*scale,conf.h*scale), "Gas sim");
//     window.setFramerateLimit(60);


//     while(window.isOpen()){
//         sf::Event event;
//         while(window.pollEvent(event)){
//             if(event.type == sf::Event::Closed){
//                 window.close();
//             }
//             if(event.type == sf::Event::KeyPressed){
//                 if(event.key.code == sf::Keyboard::Escape){
//                     wasPressed = !wasPressed;
//                 }else if(event.key.code == sf::Keyboard::Up){
//                     steps += 1;
//                 }else if(event.key.code == sf::Keyboard::Down){
//                     steps -= 1;
//                 } 
                
//                 if(debug == true){

//                 }
//             }
//             if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
//                 sim.set_piston_vel(u);
//             }else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
//                 sim.set_piston_vel(-u);
//             }else{
//                 sim.set_piston_vel(0);
//             }
//         }

//         window.clear(sf::Color::Black);
//         for(int i = 0; i < particles.size(); i++){
//             makeColor(circles[i],sim.kinetic_energy()/conf.n, particles[i]);
//             circles[i].setPosition(particles[i].pos.x*scale,particles[i].pos.y*scale);
//             window.draw(circles[i]);
//         }
//         const float px = static_cast<float>(sim.piston_x() * scale);
//         piston.setSize(sf::Vector2f(px, static_cast<float>(conf.h * scale)));
//         window.draw(piston);
//         refreshLabel(label, sim, steps);
//         window.draw(label);
//         window.display();
//         if(!wasPressed) {
//             for(int i = 0; i < steps; i++){
//                 sim.step();
//             }
//         }
//     }


//     return 0;
// }