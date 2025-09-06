#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <deque>
#include <mutex>
#include <iostream>
#include <cmath>

#define SAMPLE_RATE 44100
#define WAVE_GRAPH_SAMPLES 512


int waveType = 1, saw = 0, sine = 1, triangle = 3, sawSine = 4, sawTri = 5, sineTri = 6, threexOsc = 7;
float amplitude[3] = {1.0f, 1.0f, 1.0f};

struct Note
{
    float frequency;
    float phase;
};



std::vector<Note> activeNotes;
std::deque<float> waveBuffer;
std::mutex waveBufferMutex;

float currentTime = 0.0f;

//renderer
bool DrawButton(Rectangle bounds, const char* text)
{
    bool isPressed = false;
    Vector2 mousePoint = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePoint, bounds);

    Color buttonColor = GRAY;
    if (isHovered)
    {
        buttonColor = LIGHTGRAY;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            isPressed = true;
        }
    }

    DrawRectangleRec(bounds, buttonColor);
    DrawRectangleLinesEx(bounds, 2, DARKGRAY);

    int textWidth = MeasureText(text, 20);
    DrawText(text, bounds.x + (bounds.width - textWidth) / 2, bounds.y + (bounds.height - 20) / 2, 20, WHITE);

    return isPressed;
}



void drawWaveGraph()
{
    int x = GetRandomValue(0, 255);
    int y = GetRandomValue(0, 255);
    int z = GetRandomValue(0, 255);
    Color randomColor = { x, y, z, 255 };
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const int graphY = screenHeight - 480;
    const int graphHeight = 100;
    const int zeroLineY = graphY + graphHeight / 2;
    const float waveDrawScale = 1.0f;

    //DrawText("Graph:", 10, graphY - 30, 20, DARKGRAY);
    //DrawRectangle(0, graphY, screenWidth, graphHeight, LIGHTGRAY);
    //DrawLine(0, zeroLineY, screenWidth, zeroLineY, DARKGRAY);

    std::lock_guard<std::mutex> lock(waveBufferMutex);
    if (waveBuffer.size() > 1)
    {
        for (size_t i = 0; i < waveBuffer.size() - 1; i++)
        {
            float x1 = (float)i / WAVE_GRAPH_SAMPLES * screenWidth * waveDrawScale;
            float y1 = zeroLineY - (waveBuffer[i] * 40.0f);
            float x2 = (float)(i + 1) / WAVE_GRAPH_SAMPLES * screenWidth * waveDrawScale;
            float y2 = zeroLineY - (waveBuffer[i + 1] * 40.0f);

            if (x1 < screenWidth)
            {
                DrawLineEx({ x1, y1 }, { x2, y2 }, 3.0f, randomColor);
                //DrawLineV({ x1, y1 }, { x2, y2 }, BLUE);
            }
        }
    }
}


//calc

float getTriangleWave(float frequency, float time) {
    float sinWave = asin(sin(frequency * 2 * PI * time));
    return amplitude[2] * sinWave;
}

float getSquareWave(float frequency, float time) {
    float sinWave = sinf(frequency * 2 * PI * time);
    return amplitude[1] * (sinWave > 0.0 ? 0.2 : -0.2);
}

float getSineWave(float frequency, float time) {
    float sinWave = sinf(frequency * 2 * PI * time);
    return amplitude[0] * sinWave;
}
/*
    float getTriangleWave(float frequency, float time){


    }

*/



//callback
void audioCallback(void* bufferData, unsigned int frames)
{
    float* samples = (float*)bufferData;
    std::lock_guard<std::mutex> lock(waveBufferMutex);

    for (unsigned int i = 0; i < frames; i++)
    {
        samples[i] = 0.0f;
        for (Note& note : activeNotes)
        {
            float time = currentTime + (float)i / SAMPLE_RATE;
            if (waveType == sine) {
                samples[i] += getSineWave(note.frequency, time);
            }
            if (waveType == saw) {
                samples[i] += 2.0f * getSquareWave(note.frequency, time);
            }
            if (waveType == triangle) {
                samples[i] += getTriangleWave(note.frequency, time);
            }
            if (waveType == sawSine) {
                samples[i] += 2.0f * getSquareWave(note.frequency, time);
                samples[i] += getSineWave(note.frequency, time);
            }
            if (waveType == sineTri) {
                samples[i] += 2.0f * getSineWave(note.frequency, time);
                samples[i] += getTriangleWave(note.frequency, time);
            }
            if (waveType == sawTri) {
                samples[i] += 2.0f * getSquareWave(note.frequency, time);
                samples[i] += getTriangleWave(note.frequency, time);
            }
            if (waveType == threexOsc) {
                samples[i] += getSineWave(note.frequency, time);
                samples[i] += 2.0f * getSquareWave(note.frequency, time);
                samples[i] += getTriangleWave(note.frequency, time);
            }

            //samples[i] += sinf(note.frequency * 2.0f * PI * time);
        }

        if (!activeNotes.empty())
        {
            samples[i] /= activeNotes.size();
        }

        waveBuffer.push_back(samples[i]);
        if (waveBuffer.size() > WAVE_GRAPH_SAMPLES)
        {
            waveBuffer.pop_front();
        }
    }
    // Cập nhật thời gian sau khi xử lý xong một khung hình
    currentTime += (float)frames / SAMPLE_RATE;
}


void addNote(float frequency)
{
    for (const auto& note : activeNotes)
    {
        if (note.frequency == frequency) return;
    }
    activeNotes.push_back({ frequency, 0.0f });
}

void removeNote(float frequency)
{
    for (size_t i = 0; i < activeNotes.size(); ++i)
    {
        if (activeNotes[i].frequency == frequency)
        {
            activeNotes.erase(activeNotes.begin() + i);
            return;
        }
    }
}
void renderKeys(int keyPressed) {
    const int keys[] = {
        KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J,
        KEY_W, KEY_E, KEY_T, KEY_Y, KEY_U
    };
    const char nameOfKeys[1000][1000] = { "C4",  "D4",  "E4",
        "F4",  "G4",  "A4", "B4", "C#4", "D#4", "F#4", "G#4", "A#4", "C5"
    };
    const int count = sizeof(keys) / sizeof(keys[0]);
    for (int i = 0; i < count; i++) {
        if (keyPressed == keys[i]) {
            DrawText(nameOfKeys[i], 100, 600, 50, WHITE);
        }
    }
}
int temp = 0;
void handleInput(){
    
    const int keys[] = {
        KEY_A, KEY_S, KEY_D, KEY_F, KEY_G, KEY_H, KEY_J,
        KEY_W, KEY_E, KEY_T, KEY_Y, KEY_U
    };
    const float freqs[] = {
        261.63f, 293.66f, 329.63f, 349.23f, 392.00f, 440.00f, 493.88f, // Các nốt tự nhiên
        277.18f, 311.13f, 369.99f, 415.30f, 466.16f // Các nốt thăng
    };

    const int count = sizeof(keys) / sizeof(keys[0]);
    if (temp > 0) {
        renderKeys(temp);
    }
    for (int i = 0; i < count; i++) {
        if (IsKeyPressed(keys[i])) {
            addNote(freqs[i]);
            temp = keys[i];
        }
        if (IsKeyReleased(keys[i])) {
            removeNote(freqs[i]);
            temp = 0;
        }
    }
}




int main(void){
    const int screenWidth = 1366;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "Auralith");
    InitAudioDevice();

    AudioStream stream = LoadAudioStream(SAMPLE_RATE, 32, 1);
    SetAudioStreamCallback(stream, audioCallback);
    PlayAudioStream(stream);

    SetTargetFPS(120);
    

    while (!WindowShouldClose())
    {
        int x = GetMouseX();
        int y = GetMouseY();
        //std::cout << "x = " << x << " y = " << y << '\n';
        if (IsAudioStreamProcessed(stream))
        {
            UpdateAudioStream(stream, NULL, 0);
        }
        BeginDrawing();
        //DrawRectangle(0, 500, screenWidth, 500, DARKGRAY);
        handleInput();

        //DrawRectangle( 0, 0, screenWidth, 30, DARKGRAY);
        if (DrawButton({0, 0, 100, 30 }, "Sine")) {
            waveType = sine;
            std::cout << "ok\n";
        }
        if (DrawButton({110, 0, 100, 30}, "Saw")) {
            waveType = saw;
            std::cout << "ok\n";
        }
        if (DrawButton({220, 0, 120, 30 }, "Triangle")) {
            waveType = triangle;
            std::cout << "ok\n";
        }
        if (DrawButton({ 220 + 120 + 10, 0, 140, 30 }, "Sine + Saw")) {
            waveType = sawSine;
            std::cout << "ok\n";
        }
        if (DrawButton({ 220 + 120 + 10, 0, 140, 30 }, "Sine + Saw")) {
            waveType = sawSine;
            std::cout << "ok\n";
        }
        if (DrawButton({ 340 + 150 + 10, 0, 160, 30 }, "Sine + Triangle")) {
            waveType = sineTri;
            std::cout << "ok\n";
        }
        if (DrawButton({ 340 + 150 + 170 + 10, 0, 160, 30 }, "Saw + Triangle")) {
            waveType = sawTri;
            std::cout << "ok\n";
        }
        if (DrawButton({ 340 + 150 + 170 + 170 + 10, 0, 160, 30 }, "3xOsc")) {
            waveType = threexOsc;
            std::cout << "ok\n";
        }
        //amplitude adjust





        //preset manager
        if (DrawButton({ 1200 , 700, 160, 30 }, "Open Preset")) {
            std::cout << "ok\n";
        }
        if (DrawButton({ 1030 , 700, 160, 30 }, "Save Preset")) {
            std::cout << "ok\n";
        }
        DrawText("Auralith 0.0.1b", 10, 740, 20, WHITE);
        DrawText("KhanhTQ, 2025",1210, 740, 20, WHITE);
        ClearBackground(BLACK);
        DrawText("A, W, S, E, D, F, T, G, Y, H, J to play", 10, 50, 20, WHITE);
        DrawText(TextFormat("Voices: %i", activeNotes.size()), 10, 90, 20, WHITE);

        drawWaveGraph();

        EndDrawing();
    }

    UnloadAudioStream(stream);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}