#pragma once

#include "State.hpp"

#include <SFML/Graphics/Text.hpp>

class TitleState : public State {
    public:
        static const sf::Color Foreground;
        static const sf::Color Background;

    public:
        TitleState(StateStack &stack, Context context);
        virtual void draw() override;
        virtual bool update(sf::Time dt) override;
        virtual bool handleEvent(const sf::Event &event) override;

    private:
        sf::Text _titleText;
};
