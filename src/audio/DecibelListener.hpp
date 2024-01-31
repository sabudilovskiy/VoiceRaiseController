#pragma once
#include <SFML/Audio/SoundRecorder.hpp>
#include <functional>
#include <thread>

struct DecibelListener : private sf::SoundRecorder {
    bool Start(const std::string& device);
    void Stop();
    double GetCurDecibel();
    using sf::SoundRecorder::getAvailableDevices;
    ~DecibelListener() override;
   private:
    bool onProcessSamples(const sf::Int16 *samples, std::size_t sampleCount) override;
   private:
    std::atomic<double> db_{-80.0};
};
