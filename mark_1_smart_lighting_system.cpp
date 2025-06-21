// Smart Home Lighting Controller (Mark 1)
// Simulated Embedded System Project in C++

#include <iostream>
#include <map>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>

using namespace std;
using namespace std::chrono;

class Light {
private:
    string roomName;
    bool isOn;
    time_point<steady_clock> timerEnd;
    bool timerActive;
    int timerDuration;

public:
    Light(string name) : roomName(name), isOn(false), timerActive(false), timerDuration(0) {}

    void turnOn() {
        isOn = true;
        timerActive = false;
        cout << roomName << " light turned ON.\n";
    }

    void turnOff() {
        isOn = false;
        timerActive = false;
        cout << roomName << " light turned OFF.\n";
    }

    void setTimer(int seconds) {
        timerDuration = seconds;
        timerEnd = steady_clock::now() + seconds_cast<seconds>(seconds); // Fix: changed 'seconds_cast' to 'seconds'
        isOn = true;
        timerActive = true;
        cout << roomName << " light set to auto-off in " << seconds << " seconds.\n";
    }

    void checkTimer() {
        if (timerActive && steady_clock::now() >= timerEnd) {
            turnOff();
            timerActive = false;
        }
    }

    bool getStatus() const { return isOn; }
    string getRoomName() const { return roomName; }
};

class LightController {
private:
    map<string, Light> lights;

public:
    void addRoom(const string& room) {
        if (lights.find(room) == lights.end()) {
            lights[room] = Light(room);
            cout << "Room '" << room << "' added.\n";
        } else {
            cout << "Room already exists.\n";
        }
    }

    void turnOnLight(const string& room) {
        if (lights.count(room)) lights[room].turnOn();
        else cout << "Room not found.\n";
    }

    void turnOffLight(const string& room) {
        if (lights.count(room)) lights[room].turnOff();
        else cout << "Room not found.\n";
    }

    void setLightTimer(const string& room, int seconds) {
        if (lights.count(room)) lights[room].setTimer(seconds);
        else cout << "Room not found.\n";
    }

    void updateTimers() {
        for (auto& [name, light] : lights) {
            light.checkTimer();
        }
    }

    void showStatus() const {
        cout << "\n-- Light Status --\n";
        for (const auto& [name, light] : lights) {
            cout << name << ": " << (light.getStatus() ? "ON" : "OFF") << "\n";
        }
        cout << "-------------------\n\n";
    }

    void emergencyOverride() {
        cout << "EMERGENCY MODE ACTIVATED: Turning all lights ON!\n";
        for (auto& [name, light] : lights) {
            light.turnOn();
        }
    }
};

void printHelp() {
    cout << "\nCommands:\n"
         << "ADD [room]             - Add a new room\n"
         << "ON [room]              - Turn on light\n"
         << "OFF [room]             - Turn off light\n"
         << "TIMER [room] [sec]     - Turn on light with auto-off timer\n"
         << "STATUS                 - Show status of all lights\n"
         << "EMERGENCY              - Turn on all lights\n"
         << "HELP                   - Show this help\n"
         << "EXIT                   - Quit the program\n\n";
}

int main() {
    LightController controller;
    string input;
    printHelp();

    while (true) {
        controller.updateTimers();

        cout << "> ";
        getline(cin, input);
        controller.updateTimers();

        if (input.empty()) continue;

        string cmd;
        string arg1;
        int arg2 = 0;

        istringstream iss(input);
        iss >> cmd >> arg1 >> arg2;

        if (cmd == "ADD") controller.addRoom(arg1);
        else if (cmd == "ON") controller.turnOnLight(arg1);
        else if (cmd == "OFF") controller.turnOffLight(arg1);
        else if (cmd == "TIMER") controller.setLightTimer(arg1, arg2);
        else if (cmd == "STATUS") controller.showStatus();
        else if (cmd == "EMERGENCY") controller.emergencyOverride();
        else if (cmd == "HELP") printHelp();
        else if (cmd == "EXIT") break;
        else cout << "Invalid command. Type HELP.\n";

        this_thread::sleep_for(milliseconds(100));
    }

    return 0;
}
