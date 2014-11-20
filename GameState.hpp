#pragma once

#include "State.hpp"
#include "World.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

class GameState : public State {
    public:
        GameState(StateStack &stack, Context context);
        virtual void draw() override;
        virtual bool update(sf::Time dt) override;
        virtual bool handleEvent(const sf::Event &event) override;

    private:
        World _world;
};
