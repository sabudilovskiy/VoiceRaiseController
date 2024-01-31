#include <imgui.h>
#include <implot.h>
#include <implot_internal.h>

#include <App.hpp>
#include <DecibelListener.hpp>
#include <array>
#include <iostream>

#include "Notifier.hpp"

namespace im_hui = ImGui;
namespace im_plot = ImPlot;

using namespace std::chrono;

struct MyApp : gui::App{
    static void LoadCirilyc(){
        auto& io = im_hui::GetIO();
        io.Fonts->Clear();
        io.Fonts->AddFontFromFileTTF(
            "tmr.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }
    void PreRun() override{
        LoadCirilyc();
        im_plot::CreateContext();
        notifier.LoadFromFile("alarm.ogg");
    }
    void PlotLimit(){
        std::array<float, 2> x{0,100};
        std::array<float, 2> y{max_raise, max_raise};
        im_plot::PlotLine("Лимит", x.data(), y.data(), 2);
    }
    void PostRun() override{
        im_plot::DestroyContext();
    }
    void SelectAudioDevices(){
        for (std::size_t i = 0; i < devices.size(); i++)
        {
            if (im_hui::Selectable(devices[i].data(), selected_device == i))
            {
                selected_device = i;
            }
        }
    }
    void ToggleRecord(){
        if (im_hui::Button("Запустить запись") && selected_device != -1)
        {
            decibel_listener.Start(devices[selected_device]);
        }
        if (im_hui::Button("Остановить запись"))
        {
            decibel_listener.Stop();
        }
    }
    void PlotDecibels(){
        im_hui::VSliderFloat(" ", {20, 300}, &max_raise, 0.0f, 80.0f,
                             "%.3f");
        im_hui::SameLine();
        if (im_plot::BeginPlot("Децибелы")){
            im_plot::SetupAxesLimits(0, 100, 0, 85);
            im_plot::PlotBars("Громкость", decibels_.data(), decibels_.size(),
                              0.5);
            PlotLimit();
            im_plot::EndPlot();
        }
    }
    void NotifyIfNeed(){
        if (decibels_.back() > max_raise && !start_notify)
        {
            notifier.Start();
            start_notify = steady_clock::now();
        }
        else if (decibels_.back() < max_raise && start_notify)
        {
            auto diff = steady_clock::now() - *start_notify;
            if (diff >= 5s)
            {
                start_notify = {};
                notifier.Stop();
            }
        }
    }
    void Update() override{
        UpdateDecibels();
        if (im_hui::Begin("VCR"))
        {
            SelectAudioDevices();
            ToggleRecord();
            PlotDecibels();
            NotifyIfNeed();
            im_hui::End();
        }
    }
    void UpdateDecibels(){
        double new_db = decibel_listener.GetCurDecibel() + 80;
        if (decibels_.size() >= 100)
        {
            decibels_.erase(decibels_.begin());
        }
        decibels_.push_back(new_db);
    }
    MyApp() : App(800, 600, "VCR"){
        decibels_.reserve(100);
    }
   private:
    DecibelListener decibel_listener;
    float max_raise = 80;
    int selected_device = -1;
    std::vector<std::string> devices = DecibelListener::getAvailableDevices();
    std::vector<float> decibels_;
    Notifier notifier;
    std::optional<std::chrono::steady_clock::time_point> start_notify = {};
};
int main()
{
    MyApp myApp;
    myApp.Run();
}
