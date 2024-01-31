#include "App.hpp"

#include <imgui-SFML.h>

namespace gui{

void App::Run()
{
    window_.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window_)) return;
    PreRun();
    if (!ImGui::SFML::UpdateFontTexture()) return;
    sf::Clock deltaClock;
    while (window_.isOpen()) {
        sf::Event event{};
        while (window_.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window_, event);
            if (event.type == sf::Event::Closed) {
                window_.close();
            }
        }

        ImGui::SFML::Update(window_, deltaClock.restart());

        Update();

        window_.clear();
        ImGui::SFML::Render(window_);
        window_.display();
    }
    PostRun();

    ImGui::SFML::Shutdown();
}
App::App(int h, int w, const std::string& name) : window_(sf::VideoMode(h, w), std::string{name})
{

}
}