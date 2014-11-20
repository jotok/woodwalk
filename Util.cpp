#include "Util.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace Util {

    void centerOrigin(sf::Sprite &sprite) {
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
    }

    void centerOrigin(sf::Text &text) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(std::floor(bounds.width / 2.f), std::floor(bounds.height / 2.f));
    }

    void centerOrigin(sf::CircleShape &circle) {
        float r = circle.getRadius();
        circle.setOrigin(std::floor(r), std::floor(r));
    }

}
