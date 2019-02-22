//
// Created by salamander on 2/21/19.
//

#include "windows.h"

Window::~Window() {
    m_window.close();
}

Window::Window(const std::string& title, const sf::Vector2u& size){
    setup(title,size);
}

void Window::Create() {
    auto style = (m_fullScreen ? sf::Style::Fullscreen : sf::Style::Default);
    m_window.create({m_windowSize.x,m_windowSize.y,32},m_windowTitle, style);
}

void Window::setup(const std::string &title, const sf::Vector2u &size) {
    m_windowTitle = title;
    m_windowSize= size;
    m_fullScreen = false;
    m_isDone = false;
    Create();
}

void Window::Destroy() {
    m_window.close();
}

void Window::update() {
    sf::Event event;
    while(m_window.pollEvent(event)){
        if(event.type == sf::Event::Closed){
            Destroy();
        }
    }
}

void Window::draw(sf::Drawable &object) {
    m_window.draw(object);
}

bool Window::IsOpen() {
    return m_window.isOpen();
}

void Window::beginDraw() {
    m_window.clear(sf::Color::Black);
}

void Window::endDraw() {
    m_window.display();
}

sf::Vector2u Window::getSize() {
    return m_windowSize;
}