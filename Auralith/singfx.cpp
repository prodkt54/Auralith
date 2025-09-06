#include "raylib.h"
#include <math.h> // For sinf()
#include <iostream>

#define vPI 3.14159265359

const int screenWidth = 1366;
const int screenHeight = 768;


class Renderer {
private:
    float amplitude = 100.0f;
    float baseFrequency = 0.001f;
    float frequency = baseFrequency * 1000.0f;
    float phase_shift = 0.0f;
    float vertical_offset = screenHeight / 2.0f;
public:
    void waveformRender();
};

void Renderer::waveformRender() {
    DrawText(TextFormat("Frequency = %.2f Hz", frequency), 0, 0, 35, WHITE);
    Vector2 previous_point = { 0 };
    for (int x = 0; x < screenWidth; x++) {
        float y = amplitude * sinf(2 * vPI * baseFrequency * x + phase_shift) + vertical_offset;
        Vector2 current_point = { (float)x, y };
        if (x > 0)
        {
            DrawLineV(previous_point, current_point, WHITE);
        }
        previous_point = current_point;
    }
    phase_shift += 0.1f;
}



class generateSound {
private:
    const int sampleRate = 44100;
    const float frequency = 440.0f;
    const float duration = 2.0f;
    const int samplesCount = (int)(sampleRate * duration);
public:
    void sineGenerator();
};



void generateSound::sineGenerator() {
    short* samples = new short[samplesCount];

    for (int i = 0; i < samplesCount; i++) {
        float t = (float)i / sampleRate;           // thời gian
        float value = sinf(2.0f * vPI * frequency * t);
        samples[i] = (short)(value * 32767);      // chuyển sang short
    }
    AudioStream stream = LoadAudioStream(1, sampleRate, 16);
    UpdateAudioStream(stream, samples, samplesCount);
    PlayAudioStream(stream);
}






//int main() {
//    InitAudioDevice();
//    InitWindow(screenWidth, screenHeight, "Sine Wave");
//    SetTargetFPS(60);
//    Renderer renderer;
//    //generateSound gS;
//    //gS.sineGenerator();
//
//
//    const int sampleRate = 44100;
//    const float frequency = 440.0f; // A4
//    const float duration = 2.0f;
//    const int bufferSize = 1024;    // chunk nhỏ
//    const int totalSamples = (int)(sampleRate * duration);
//    AudioStream stream = LoadAudioStream(1, sampleRate, 16);
//    PlayAudioStream(stream);
//
//    short audioBuffer[bufferSize];
//    int sampleIndex = 0;
//
//    while (sampleIndex < totalSamples) {
//        // --- tạo chunk sine wave ---
//        for (int i = 0; i < bufferSize && sampleIndex + i < totalSamples; i++) {
//            float t = (float)(sampleIndex + i) / sampleRate;
//            float value = sinf(2.0f * vPI * frequency * t);
//            audioBuffer[i] = (short)(value * 32767);
//        }
//
//        // --- chỉ update khi buffer trống ---
//        if (IsAudioStreamProcessed(stream)) {
//            UpdateAudioStream(stream, audioBuffer, bufferSize);
//            sampleIndex += bufferSize;
//        }
//    }
//
//
//
//    while (!WindowShouldClose()){
//        BeginDrawing();
//        ClearBackground(BLACK);
//        renderer.waveformRender();
//        EndDrawing();
//        
//    }
//
//    CloseWindow();
//
//    return 0;
//}