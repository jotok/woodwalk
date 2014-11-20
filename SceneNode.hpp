#pragma once

#include "Category.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <memory>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;
        typedef std::pair<SceneNode*, SceneNode*> Pair;

    public:
        explicit SceneNode(Category::Type category = Category::None);
        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode &node);
        void update(sf::Time dt, CommandQueue &commands);
        sf::Vector2f getWorldPosition() const;
        sf::Transform getWorldTransform() const;
        void onCommand(const Command &command, sf::Time dt);
        virtual unsigned int getCategory() const;

        void removeMarked();
        void removeIf(std::function<bool(const SceneNode&)> fn);
        virtual sf::FloatRect getBoundingRect() const;
        virtual bool isMarkedForRemoval() const;
        std::vector<Ptr> _children;

    private:
        virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
        void updateChildren(sf::Time dt, CommandQueue &commands);
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
        virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
        void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;
        void drawBoundingRect(sf::RenderTarget &target, sf::RenderStates states) const;

    private:
        SceneNode *_parent;
        Category::Type _defaultCategory;

};
