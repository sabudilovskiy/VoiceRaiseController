#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

namespace gui
{

struct App
{
    void Run();
    virtual void PreRun(){}
    virtual void PostRun(){}
    virtual void Update() = 0;
    App(int h, int w, const std::string& name);
   private:
    sf::RenderWindow window_;
};

}