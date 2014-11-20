#pragma once

#include "BloomEffect.hpp"
#include "Data.hpp"
#include "GradientEffect.hpp"
#include "ResourceHolder.hpp"
#include "Resources.hpp"
#include "ShapeNode.hpp"
#include "CommandQueue.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>
#include <vector>
#include <random>

namespace sf {
    class RenderTarget;
}

class World : private sf::NonCopyable {
    public:
        static const float AccelerationRate;
        static const float MaxSpeed;
        static const float SpawnAreaWidth;
        static const float SunRadius;
        static const float SunCenterAmplitude;
        static const float SecondsPerGameDay;
        static const sf::Color NightColor;
        static const sf::Color DayColor;

    public:
        World(sf::RenderTarget &outputTarget, TextureHolder &textures, ShaderHolder &shaders);
        void update(sf::Time dt);
        void draw();
        CommandQueue& getCommandQueue();

    private:
        enum Layer {
            SkyLayer,
            TreeBackLayer,
            TreeMiddleLayer,
            TreeFrontLayer,
            LayerCount
        };

    private:
        void spawnTrees(SceneNode &node, Category::Type type, const std::vector<TreeData> &data, int n, float x, float y, float width, float height);
        bool isOffLeft(const SceneNode &node);

    private:
        sf::RenderTarget& _target;
        std::array<sf::RenderTexture*, 2> _sceneTextures;
        sf::RenderTexture _skyTexture;
        sf::View _worldView;
        sf::View _skyView;
        TextureHolder& _textures;


        SceneNode _sceneGraph;
        std::array<SceneNode*, LayerCount> _sceneLayers;
        CommandQueue _commandQueue;

        sf::FloatRect _worldBounds;

        sf::RectangleShape _dayRect;
        sf::RectangleShape _nightRect;
        ShapeNode<sf::RectangleShape>::Ptr _day;
        ShapeNode<sf::RectangleShape>::Ptr _night;
        ShapeNode<sf::CircleShape>* _sun;
        int _dayOfYear;
        float _timeOfDay;

        std::vector<TreeLayerData> _treeLayers;

        std::default_random_engine _rng;
        std::vector<std::poisson_distribution<int>> _poisson;

        double _speed;
        std::array<float, LayerCount - TreeBackLayer> _spawnLine;

        BloomEffect _bloomEffect;
        GradientEffect _gradientEffect;
};
