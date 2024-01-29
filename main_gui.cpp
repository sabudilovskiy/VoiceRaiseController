#include <hello_imgui.h>

#include <iostream>

#include "DecibelListener.hpp"

namespace im_hui = ImGui;
namespace hello_im_hui = HelloImGui;

void LoadCirilyc()
{
    auto& io = im_hui::GetIO();
    io.FontDefault = io.Fonts->AddFontFromFileTTF(
        "tmr.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesCyrillic());
}

int main()
{
    DecibelListener decibel_listener;
    auto devices = DecibelListener::getAvailableDevices();
    float max_raise = 80;
    int selected_device = -1;
    std::vector<float> decibels_;
    decibels_.reserve(100);
    auto update_decibels = [&](){
        double new_db = decibel_listener.GetCurDecibel() + 80;
      if (decibels_.size() >= 100)
      {
          decibels_.erase(decibels_.begin());
      }
      decibels_.push_back(new_db);
    };
    auto guiFunction = [&]()
    {
        auto start = std::chrono::steady_clock::now();
        im_hui::ShowDemoWindow();
        for (std::size_t i = 0; i < devices.size(); i++)
        {
            if (im_hui::Selectable(devices[i].data(), selected_device == i))
            {
                selected_device = i;
            }
        }
        if (im_hui::Button("Запустить запись") && selected_device != -1)
        {
            decibel_listener.Start(devices[selected_device]);
        }
        if (im_hui::Button("Остановить запись")){
            decibel_listener.Stop();
        }
        im_hui::SliderFloat("MaxRaize (0 -> 80)", &max_raise, 0.0f, 80.0f, "%.3f");
        update_decibels();
        im_hui::PlotLines("Decibels", decibels_.data(), decibels_.size(),  0, NULL, 0, 80, ImVec2(0, -80.0f));
        if (decibels_.back() > max_raise){
            im_hui::Text("GROMKO PIZDA");
        }
        auto end = std::chrono::steady_clock::now();
        auto ms = end - start;
        std::this_thread::sleep_for(std::chrono::milliseconds{16} - ms);
    };
    hello_im_hui::RunnerParams params;
    params.callbacks.ShowGui = guiFunction;
    params.appWindowParams.windowGeometry.size = {1366, 768};
    params.appWindowParams.windowTitle = "VoiceRaiseController";
    params.callbacks.LoadAdditionalFonts = LoadCirilyc;
    hello_im_hui::Run(params);
    decibel_listener.Stop();
    return 0;
}
