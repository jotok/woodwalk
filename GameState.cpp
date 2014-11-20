#include "GameState.hpp"

#include "Data.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack &stack, Context context)
: State(stack, context)
, _world(*context.window, *context.textures, *context.shaders)
{}

void GameState::draw() {
    _world.draw();
}

bool GameState::update(sf::Time dt) {
    _world.update(dt);

    return true;
}

bool GameState::handleEvent(const sf::Event &event) {
    // CommandQueue &commands = _world.getCommandQueue();
    // move this to World
    // handle commands

    return true;
}
