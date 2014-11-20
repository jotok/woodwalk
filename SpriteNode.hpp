#pragma once

#include "SceneNode.hpp"

#include <SFML/Graphics/Sprite.hpp>

class SpriteNode : public SceneNode {
    public:
        explicit SpriteNode(const sf::Texture &texture, Category::Type type = Category::None);
        SpriteNode(const sf::Texture &texture, const sf::IntRect &textureRect, Category::Type type = Category::None);
        SpriteNode(const sf::Sprite &sprite, Category::Type type = Category::None);

        virtual sf::FloatRect getBoundingRect() const override;

    private:
        virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

    private:
        sf::Sprite _sprite;
};
