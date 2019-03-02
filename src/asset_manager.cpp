#include "asset_manager.h"

void AssetManager::loadTexture(std::string name, std::string fileName)
{
    sf::Texture tex;
    if(tex.loadFromFile(fileName))
    {
        m_textures[name] = tex;
    }
}

sf::Texture &AssetManager::getTexture(std::string name)
{
    return m_textures.at(name);
}

void AssetManager::loadFont(std::string name, std::string fileName)
{
    sf::Font font;
    if(font.loadFromFile(fileName))
    {
        m_fonts[name] = font;
    }
}

sf::Font &AssetManager::getFont(std::string name)
{
    return m_fonts.at(name);
}

void AssetManager::loadSound(std::string name, std::string fileName)
{
    sf::SoundBuffer buffer;
    if(buffer.loadFromFile(fileName))
    {
        m_soundBuffers[name] = buffer;
        sf::Sound sound;
        sound.setBuffer(m_soundBuffers[name]);
        m_sounds[name] = sound;
    }
}

sf::Sound &AssetManager::getSound(std::string name)
{
    return m_sounds.at(name);
}