//
// Created by salamander on 2/21/19.
//

#include <SFML/Graphics.hpp>
#ifndef DEPOKER_WINDOWS_H
#define DEPOKER_WINDOWS_H

class Window{
private:
    sf::RenderWindow m_window;
    sf::Vector2u m_windowSize;
    sf::String m_windowTitle;
    bool m_fullScreen;
    bool m_isDone;
    void Create();

public:
    Window(){};
    Window(const std::string& title, const sf::Vector2u& size);
    void setup(const std::string& title, const sf::Vector2u& size);
    ~Window();
    void Destroy();
    void update();
    void draw(sf::Drawable& object);
    bool IsOpen();
    void beginDraw();
    void endDraw();
    sf::Vector2u getSize();
};

#endif //DEPOKER_WINDOWS_H
