#pragma once

#include "Data.hpp"
#include "StateStack.hpp"
#include "ResourceHolder.hpp"
#include "Resources.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>

class Application 
{
    public:
        Application();
        void run();

    private:
        void processInput();
        void update(sf::Time dt);
        void render();

        void registerStates();

    private:
        static const sf::Time TimePerFrame;

        sf::RenderWindow _window;
        TextureHolder _textures;
        FontHolder _fonts;
        ShaderHolder _shaders;
        // MusicPlayer _music;
        // SoundPlayer _sounds;
        StateStack _stateStack;
};
