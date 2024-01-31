#pragma
#include <SFML/audio.hpp>

struct Notifier
{
    void Start();
    void Stop();
    void LoadFromFile(const std::string& file_path);
    ~Notifier();
   private:
    sf::Sound sound;
    sf::SoundBuffer buffer;
};
