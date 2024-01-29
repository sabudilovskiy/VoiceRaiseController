#include "DecibelListener.hpp"

#include <SFML/Audio.hpp>
#include <span>
#include <thread>
#include <vector>

double calculateDecibels(std::span<const sf::Int16> samples)
{
    // Вычисляем средний квадрат амплитуды (RMS)
    double rms = 0.0;

    for (auto sample : samples)
    {
        rms += sample * sample;
    }
    rms = std::sqrt(rms / samples.size());

    // Максимальная амплитуда для 16-битного аудио (максимальное значение Int16)
    constexpr double maxAmplitude = std::numeric_limits<sf::Int16>::max();

    // Вычисляем уровень громкости в децибелах
    // Формула: 20 * log10(rms / maxAmplitude)
    double decibels = 20.0 * std::log10(rms / maxAmplitude);

    return decibels;
}

bool DecibelListener::Start(const std::string& device)
{
    stop();
    if (!setDevice(device)){
        return false;
    }
    return start();
}
void DecibelListener::Stop()
{
    stop();
}
bool DecibelListener::onProcessSamples(const sf::Int16* samples,
                                     std::size_t sampleCount)
{
    db_.store(calculateDecibels({samples, sampleCount}), std::memory_order_release);
    return true;
}
double DecibelListener::GetCurDecibel()
{
    return db_.load(std::memory_order_acquire);
}