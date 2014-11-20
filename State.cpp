#include "State.hpp"
#include "StateStack.hpp"

State::Context::Context(sf::RenderWindow &window, 
                        TextureHolder &textures, 
                        FontHolder &fonts,
                        ShaderHolder &shaders)
: window(&window)
, textures(&textures)
, fonts(&fonts)
, shaders(&shaders)
{}

State::State(StateStack &stack, Context context)
: _stack{&stack}
, _context{context}
{}

State::~State() {}

void State::requestStackPush(State::ID id) {
    _stack->pushState(id);
}

void State::requestStackPop() {
    _stack->popState();
}

void State::requestStateClear() {
    _stack->clearStates();
}

State::Context State::getContext() const {
    return _context;
}
