#pragma once

#include "State.hpp"

class MenuState : public State {
    public:
        MenuState(StateStack &stack, Context context);

        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleVent(const sf::Event &event);
}
