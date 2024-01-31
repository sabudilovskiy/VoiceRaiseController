//
// Created by Andrei Budilovskii on 31.01.2024.
//

#include "Notifier.hpp"
void Notifier::Start()
{
    sound.setVolume(100);
    sound.setLoop(true);
    sound.play();
}
void Notifier::LoadFromFile(const std::string& file_path)
{
    if (!buffer.loadFromFile(file_path)){
        throw std::runtime_error("not founded sound");
    }
    sound.setBuffer(buffer);

}

void Notifier::Stop()
{
    sound.play();
    sound.pause();
    sound.stop();
}

Notifier::~Notifier(){
    sound.stop();
}