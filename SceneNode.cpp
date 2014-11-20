#include "SceneNode.hpp"
#include "Command.hpp"
#include "Util.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>

SceneNode::SceneNode(Category::Type category)
: _children{}
, _parent{nullptr}
, _defaultCategory{category}
{}

void SceneNode::attachChild(Ptr child) {
    child->_parent = this;
    _children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode &node) {
    auto found = std::find_if(_children.begin(), _children.end(),
                              [&](Ptr &p) { return p.get() == &node; });
    assert(found != _children.end());

    Ptr result = std::move(*found);
    result->_parent = nullptr;
    _children.erase(found);
    return result;
}

void SceneNode::update(sf::Time dt, CommandQueue &commands) {
    updateCurrent(dt, commands);
    updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time, CommandQueue&) {
    // do nothing by default
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue &commands) {
    for (Ptr &child: _children)
        child->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    // Apply transform of current node
    states.transform *= getTransform();

    // Draw node and children with changed transform
    drawCurrent(target, states);
    drawChildren(target, states);

    // Draw bounding rectangle - disabled by default
    // drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const {
    // Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const {
    for (const Ptr &child: _children)
        child->draw(target, states);
}

void SceneNode::drawBoundingRect(sf::RenderTarget &target, sf::RenderStates) const {
    sf::FloatRect rect = getBoundingRect();

    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2f(rect.left, rect.top));
    shape.setSize(sf::Vector2f(rect.width, rect.height));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);

    target.draw(shape);
}

sf::Vector2f SceneNode::getWorldPosition() const {
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const {
    sf::Transform transform = sf::Transform::Identity;

    for (const SceneNode *node = this; node != nullptr; node = node->_parent)
        transform = node->getTransform() * transform;

    return transform;
}

void SceneNode::onCommand(const Command &command, sf::Time dt) {
    // Command current node if category matches
    if (command.category & getCategory())
        command.action(*this, dt);

    // Command children
    for (Ptr &child: _children)
        child->onCommand(command, dt);
}

unsigned int SceneNode::getCategory() const {
    return _defaultCategory;
}

void SceneNode::removeMarked() {
    // remove marked children
    auto markedBegin = std::remove_if(_children.begin(), _children.end(), 
                                      std::mem_fn(&SceneNode::isMarkedForRemoval));
    _children.erase(markedBegin, _children.end());

    // recursive call to children
    std::for_each(_children.begin(), _children.end(),
                  std::mem_fn(&SceneNode::removeMarked));    
}

void SceneNode::removeIf(std::function<bool(const SceneNode&)> fn) {
    auto pfn = [fn](const Ptr& p) { return fn(*p); };

    auto remBegin = std::remove_if(_children.begin(), _children.end(), pfn);
    _children.erase(remBegin, _children.end());
}

sf::FloatRect SceneNode::getBoundingRect() const {
    return sf::FloatRect();
}

bool SceneNode::isMarkedForRemoval() const {
    return false;
}
