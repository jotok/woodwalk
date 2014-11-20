#include "TitleState.hpp"

#include "ResourceHolder.hpp"
#include "Util.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

const sf::Color TitleState::Foreground = sf::Color::White;

TitleState::TitleState(StateStack &stack, Context context)
: State{stack, context}
, _titleText{}
{
    _titleText.setFont(context.fonts->get(Fonts::Title));
    _titleText.setString("woodwalk");
    _titleText.setCharacterSize(24);
    _titleText.setColor(TitleState::Foreground);

    Util::centerOrigin(_titleText);
    _titleText.setPosition(sf::Vector2f{context.window->getSize() / 2u});

}

void TitleState::draw() {
    sf::RenderWindow &window = *getContext().window;
    window.draw(_titleText);

}

bool TitleState::update(sf::Time dt) {
    return true;
}

bool TitleState::handleEvent(const sf::Event &event) {
    if (event.type == sf::Event::KeyReleased) {
        requestStackPop();
        requestStackPush(State::ID::Game);
    }

    return true;
}
