#include "Application.hpp"
#include "GameState.hpp"
#include "TitleState.hpp"

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window.hpp>

#include <iostream>

const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

//: _window{sf::VideoMode(1024, 764), "woodwalk", sf::Style::Close}
Application::Application()
: _window{sf::VideoMode(1024, 764), "woodwalk", sf::Style::Fullscreen}
, _textures{}
, _fonts{}
, _shaders{}
, _stateStack{State::Context{_window, _textures, _fonts, _shaders}}
{
    _window.setKeyRepeatEnabled(false);
    _window.setVerticalSyncEnabled(true);

    _fonts.load(Fonts::Title, "/usr/share/fonts/TTF/DejaVuSerif.ttf");

    _textures.load(Textures::Trees, "Resources/VIDEOgameJOSH5.png");

    _shaders.load(Shaders::BrightnessPass, "Shaders/Fullpass.vert", "Shaders/Brightness.frag");
    _shaders.load(Shaders::DownSamplePass, "Shaders/Fullpass.vert", "Shaders/DownSample.frag");
    _shaders.load(Shaders::GaussianBlurPass, "Shaders/Fullpass.vert", "Shaders/GaussianBlur.frag");
    _shaders.load(Shaders::AddPass, "Shaders/Fullpass.vert", "Shaders/Add.frag");
    _shaders.load(Shaders::Gradient, "Shaders/Fullpass.vert", "Shaders/Gradient.frag");

    registerStates();
    _stateStack.pushState(State::ID::Title);
}

void Application::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (_window.isOpen()) {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;

        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            // Check inside this loop, because stack might be empty before update() call
            if (_stateStack.isEmpty())
                _window.close();

            render();
        }
    }
}

void Application::processInput() {
    sf::Event event;

    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            _window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            _window.close();

        }

        _stateStack.handleEvent(event);
    }
}

void Application::update(sf::Time dt) {
    _stateStack.update(dt);
}

void Application::render() {
    _window.clear();

    _stateStack.draw();

    _window.display();
}

void Application::registerStates() {
    _stateStack.registerState<TitleState>(State::ID::Title);
    _stateStack.registerState<GameState>(State::ID::Game);
}
