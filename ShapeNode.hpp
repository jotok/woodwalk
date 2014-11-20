#pragma once

#include "SceneNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

template <typename T>
class ShapeNode : public SceneNode {
    public:
        typedef std::unique_ptr<ShapeNode<T>> Ptr;

    public: 
        ShapeNode(T& shape);
        ShapeNode();
        T& getShape();

    private:
		virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        T _shape;
};

template <typename T>
ShapeNode<T>::ShapeNode(T& shape) 
: _shape(shape)
{}

template <typename T>
ShapeNode<T>::ShapeNode()
: _shape(T())
{}

template <typename T>
T& ShapeNode<T>::getShape() {
    return _shape;
}

template <typename T>
void ShapeNode<T>::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(_shape, states);
}
