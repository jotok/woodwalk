#include "SpriteNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

SpriteNode::SpriteNode(const sf::Texture &texture, Category::Type type)
: SceneNode{type}
, _sprite{texture}
{}

SpriteNode::SpriteNode(const sf::Texture &texture, const sf::IntRect &textureRect, Category::Type type)
: SceneNode{type} 
, _sprite(texture, textureRect)
{}

SpriteNode::SpriteNode(const sf::Sprite &sprite, Category::Type type)
: SceneNode{type}
, _sprite{sprite}
{}

sf::FloatRect SpriteNode::getBoundingRect() const {
    return getWorldTransform().transformRect(_sprite.getGlobalBounds());
}

void SpriteNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(_sprite, states);
}
