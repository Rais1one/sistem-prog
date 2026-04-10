#include <iostream>
#include <functional>
#include <vector>
#include <map>
#include <string>
#include <windows.h>
#include <conio.h>

enum class KeyEventType {
    KeyPressed,
    KeyReleased
};

class KeyboardEvent {
public:
    KeyEventType type;
    char key;
    int modifiers;

    KeyboardEvent(KeyEventType t, char k, int m = 0) : type(t), key(k), modifiers(m) {}

    std::string getTypeString() const {
        return (type == KeyEventType::KeyPressed) ? "НАЖАТА" : "ОТПУЩЕНА";
    }
};

class Event {
private:
    std::vector<std::function<void(const KeyboardEvent&)>> handlers;

public:
    void subscribe(std::function<void(const KeyboardEvent&)> handler) {
        handlers.push_back(handler);
    }

    void emit(const KeyboardEvent& event) {
        for (const auto& handler : handlers) {
            handler(event);
        }
    }
};

class Keyboard {
private:
    std::map<char, Event> keyPressedEvents;
    std::map<char, Event> keyReleasedEvents;
    std::map<char, bool> keyStates;
    bool running;

public:
    Keyboard() : running(true) {}

    void onKeyPressed(char key, std::function<void(const KeyboardEvent&)> handler) {
        keyPressedEvents[key].subscribe(handler);
    }

    void onKeyReleased(char key, std::function<void(const KeyboardEvent&)> handler) {
        keyReleasedEvents[key].subscribe(handler);
    }

    void pressKey(char key, int modifiers = 0) {
        if (!keyStates[key]) {
            keyStates[key] = true;
            KeyboardEvent event(KeyEventType::KeyPressed, key, modifiers);

            if (keyPressedEvents.find(key) != keyPressedEvents.end()) {
                keyPressedEvents[key].emit(event);
            }
        }
    }

    void releaseKey(char key, int modifiers = 0) {
        if (keyStates[key]) {
            keyStates[key] = false;
            KeyboardEvent event(KeyEventType::KeyReleased, key, modifiers);

            if (keyReleasedEvents.find(key) != keyReleasedEvents.end()) {
                keyReleasedEvents[key].emit(event);
            }
        }
    }

    bool isKeyPressed(char key) const {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second;
    }

    void startListening() {
        std::cout << "Начинаем прослушивание клавиатуры..." << std::endl;
        std::cout << "Нажмите ESC для выхода" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        while (running) {
            if (_kbhit()) {
                char key = _getch();

                if (key == 27) { // ESC
                    running = false;
                    std::cout << "\nВыход из программы..." << std::endl;
                    break;
                }

                key = toupper(key);
                pressKey(key);

                // Небольшая задержка для имитации отпускания
                Sleep(100);
                releaseKey(key);
            }
            Sleep(10);
        }
    }

    void stopListening() {
        running = false;
    }
};

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Keyboard keyboard;


    keyboard.onKeyPressed('A', [](const KeyboardEvent& e) {
        std::cout << "[СОБЫТИЕ] Клавиша A " << e.getTypeString() << std::endl;
        });

    keyboard.onKeyReleased('A', [](const KeyboardEvent& e) {
        std::cout << "[СОБЫТИЕ] Клавиша A " << e.getTypeString() << std::endl;
        });

    int pressCount = 0;
    keyboard.onKeyPressed('B', [&pressCount](const KeyboardEvent& e) {
        pressCount++;
        std::cout << "[СОБЫТИЕ] Клавиша B нажата " << pressCount << " раз" << std::endl;
        });

    keyboard.onKeyPressed('C', [](const KeyboardEvent& e) {
        std::cout << "[СОБЫТИЕ] Клавиша C нажата" << std::endl;
        });

    keyboard.onKeyReleased('C', [](const KeyboardEvent& e) {
        std::cout << "[СОБЫТИЕ] Клавиша C отпущена" << std::endl;
        });

    keyboard.onKeyPressed('Z', [](const KeyboardEvent& e) {
        std::cout << "[СОБЫТИЕ] Клавиша Z нажата" << std::endl;
        });

    keyboard.onKeyPressed('X', [](const KeyboardEvent& e) {
        std::cout << "[СОБЫТИЕ] Клавиша X нажата" << std::endl;
        });

    keyboard.startListening();

    return 0;
}