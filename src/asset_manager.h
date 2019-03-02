#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <map>
#include <string>

class AssetManager
{
public:
    AssetManager() {}
    ~AssetManager() {}

    void loadTexture(std::string name, std::string fileName);
    sf::Texture &getTexture(std::string name);

    void loadFont(std::string name, std::string fileName);
    sf::Font &getFont(std::string name);

    void loadSound(std::string name, std::string fileName);
    sf::Sound &getSound(std::string name);

private:
    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::Font> m_fonts;
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
    std::map<std::string, sf::Sound> m_sounds;
};
