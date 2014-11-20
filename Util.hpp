#pragma once

namespace sf {
    class CircleShape;
    class Sprite;
    class Text;
}

namespace Util {
    void centerOrigin(sf::Sprite &sprite);
    void centerOrigin(sf::Text &text);
    void centerOrigin(sf::CircleShape &circle);
}
