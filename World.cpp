#include "World.hpp"

#include "PostEffect.hpp"
#include "Util.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <functional>

#define SECONDS_PER_DAY (24 * 60 * 60)

// speeds in pixels per second

const float World::AccelerationRate = 5.f;
const float World::MaxSpeed = 100.f;
const float World::SpawnAreaWidth = 500.f;
const float World::SunRadius = 50.f;
const float World::SunCenterAmplitude = 100.f;
const float World::SecondsPerGameDay = 60.f;

const sf::Color World::NightColor = {30, 30, 30};
const sf::Color World::DayColor = {190, 190, 190};

World::World(sf::RenderTarget &outputTarget, TextureHolder& textures, ShaderHolder& shaders)
: _target{outputTarget}
, _sceneTextures{}
, _skyTexture{}
, _worldView{outputTarget.getDefaultView()}
, _skyView{}
, _textures{textures}
, _sceneGraph{}
, _sceneLayers{}
, _day{nullptr}
, _night{nullptr}
, _dayOfYear{0}
, _timeOfDay{0.f}
, _rng{}
, _speed{0.0}
, _bloomEffect{shaders}
, _gradientEffect{shaders, DayColor, NightColor}
{
    sf::Vector2u targetSize = _target.getSize();
    sf::Vector2f targetSizef = static_cast<sf::Vector2f>(_target.getSize());

    _sceneTextures[0] = new sf::RenderTexture();
    _sceneTextures[1] = new sf::RenderTexture();
    _sceneTextures[0]->create(targetSize.x, targetSize.y);
    _sceneTextures[1]->create(targetSize.x, targetSize.y);
    _skyTexture.create(targetSize.x, 3 * targetSize.y);

    GradientEffect effect(shaders, DayColor, NightColor);

    _skyView.setSize(targetSizef);

    srand(time(NULL));

    auto size = _worldView.getSize();
    _worldBounds = {0.f, 0.f, size.x + 2 * SpawnAreaWidth, size.y};

    SceneNode::Ptr skyLayer{new SceneNode(Category::SkyLayer)};
    _sceneLayers[SkyLayer] = skyLayer.get();
    _sceneGraph.attachChild(std::move(skyLayer));

    SceneNode::Ptr treeBackLayer{new SceneNode(Category::None)};
    _sceneLayers[TreeBackLayer] = treeBackLayer.get();
    _sceneGraph.attachChild(std::move(treeBackLayer));

    SceneNode::Ptr treeMiddleLayer{new SceneNode(Category::None)};
    _sceneLayers[TreeMiddleLayer] = treeMiddleLayer.get();
    _sceneGraph.attachChild(std::move(treeMiddleLayer));

    SceneNode::Ptr treeFrontLayer{new SceneNode(Category::None)};
    _sceneLayers[TreeFrontLayer] = treeFrontLayer.get();
    _sceneGraph.attachChild(std::move(treeFrontLayer));

    // _dayRect.setSize({_worldBounds.width, _worldBounds.height});
    // _dayRect.setFillColor(DayColor);
    // _dayRect.move(0, _worldBounds.height / 2);

    // _nightRect.setSize({_worldBounds.width, _worldBounds.height});
    // _nightRect.setFillColor(NightColor);
    // _nightRect.move(0, -_worldBounds.height / 2);

    // _day = ShapeNode<sf::RectangleShape>::Ptr(new ShapeNode<sf::RectangleShape>(_dayRect));
    // _night = ShapeNode<sf::RectangleShape>::Ptr(new ShapeNode<sf::RectangleShape>(_nightRect));
    // _sceneLayers[SkyLayer]->attachChild(std::move(_day));
    // _sceneLayers[SkyLayer]->attachChild(std::move(_night));

    sf::CircleShape sunShape;
    sunShape.setRadius(SunRadius);
    Util::centerOrigin(sunShape);
    sunShape.setOrigin(0, _worldBounds.height);
    sunShape.move(std::floor(size.x / 2.f), size.y);
    sunShape.setFillColor({255, 255, 255});
    auto sun = ShapeNode<sf::CircleShape>::Ptr(new ShapeNode<sf::CircleShape>(sunShape));
    _sun = sun.get();
    _sceneLayers[SkyLayer]->attachChild(std::move(sun));

    initTreeLayers(_treeLayers);

    for (int i = TreeBackLayer; i < LayerCount; i++) {
        Category::Type type = (i == TreeBackLayer) ? Category::TreeBackLayer :
                              (i == TreeMiddleLayer) ? Category::TreeMiddleLayer : 
                                                       Category::TreeFrontLayer;

        // first fill in the screen

        TreeLayerData layerData = _treeLayers[i - TreeBackLayer];

        float width = _worldBounds.width;
        float height = layerData.fheight * _worldBounds.height;
        float y = layerData.fy * _worldBounds.height;

        std::poisson_distribution<int> temprng(layerData.spawnRate(width, height));
        int n = temprng(_rng);
        spawnTrees(*_sceneLayers[i], type, layerData.trees, n, 0.f, y, width, height);

        // set the member rngs

        std::poisson_distribution<int> p(layerData.spawnRate(SpawnAreaWidth, height));
        _poisson.push_back(p);

        _spawnLine[i - TreeBackLayer] = width;
    }
}

void World::update(sf::Time dt) {
    // sun

    _timeOfDay += dt.asSeconds() * SECONDS_PER_DAY / SecondsPerGameDay;
    sf::CircleShape& sunShape = _sun->getShape();

    // until i can sync the light with the height
    // if (_timeOfDay > SECONDS_PER_DAY) {
    //     _timeOfDay -= SECONDS_PER_DAY;
    //     _dayOfYear = (_dayOfYear + 1) % 365;

    //     sunShape.move(0.f, sin(2 * M_PI * _dayOfYear / 365));
    // }

    float sunAngle = 360 * _timeOfDay / SECONDS_PER_DAY;
    sunShape.setRotation(sunAngle);
    sunAngle = M_PI / 2.0 - sunAngle * 2 * M_PI / 360.0;
    _gradientEffect.setScale(0.8 - 0.7 * sin(sunAngle));

    // trees

    for (int i = 1; i < LayerCount; i++) {
        auto layer = _sceneLayers[i];
        auto fn = [this](const SceneNode &node) { return this->isOffLeft(node); };
        layer->removeIf(fn);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && _speed < MaxSpeed) {
        _speed += AccelerationRate * dt.asSeconds();
    } else if (_speed > 0) {
        _speed -= AccelerationRate * dt.asSeconds();
    }

    if (_speed > 0) {

        double speed = -_speed * _treeLayers[0].speedMultiplier;

        Command command;
        command.category = Category::TreeBackLayer;
        command.action = derivedAction<SceneNode>([speed](SceneNode &node, sf::Time) {
            node.move(speed, 0);
        });

        _commandQueue.push(command);

        _spawnLine[0] += speed;
        if (_spawnLine[0] < _worldBounds.width - SpawnAreaWidth) {
            _spawnLine[0] = _worldBounds.width;

            TreeLayerData layerData = _treeLayers[0];
            float width = SpawnAreaWidth;
            float height = layerData.fheight * _worldBounds.height;
            float x = _worldBounds.width - SpawnAreaWidth;
            float y = layerData.fy * _worldBounds.height;
            int n = _poisson[0](_rng);

            spawnTrees(*_sceneLayers[TreeBackLayer],
                       Category::TreeBackLayer, 
                       layerData.trees, n, x, y, width, height);

        }

        speed = -_speed * _treeLayers[1].speedMultiplier;

        Command command2;
        command.category = Category::TreeMiddleLayer;
        command.action = derivedAction<SceneNode>([speed](SceneNode &node, sf::Time) {
            node.move(speed, 0);
        });

        _commandQueue.push(command);

        _spawnLine[1] += speed;

        if (_spawnLine[1] < _worldBounds.width - SpawnAreaWidth) {
            _spawnLine[1] = _worldBounds.width;

            TreeLayerData layerData = _treeLayers[1];
            float width = SpawnAreaWidth;
            float height = layerData.fheight * _worldBounds.height;
            float x = _worldBounds.width - SpawnAreaWidth;
            float y = layerData.fy * _worldBounds.height;
            int n = _poisson[1](_rng);

            spawnTrees(*_sceneLayers[TreeMiddleLayer],
                       Category::TreeMiddleLayer, 
                       layerData.trees, n, x, y, width, height);
        }

        speed = -_speed * _treeLayers[2].speedMultiplier;

        Command command3;
        command.category = Category::TreeFrontLayer;
        command.action = derivedAction<SceneNode>([speed](SceneNode &node, sf::Time) {
            node.move(speed, 0);
        });

        _commandQueue.push(command);

        _spawnLine[2] += speed;

        if (_spawnLine[2] < _worldBounds.width - SpawnAreaWidth) {
            _spawnLine[2] = _worldBounds.width;

            TreeLayerData layerData = _treeLayers[2];
            float width = SpawnAreaWidth;
            float height = layerData.fheight * _worldBounds.height;
            float x = _worldBounds.width - SpawnAreaWidth;
            float y = layerData.fy * _worldBounds.height;
            int n = _poisson[2](_rng);

            spawnTrees(*_sceneLayers[TreeFrontLayer],
                       Category::TreeFrontLayer, 
                       layerData.trees, n, x, y, width, height);

        }

    }

    // Forward commands to scene graph
    while (!_commandQueue.isEmpty())
        _sceneGraph.onCommand(_commandQueue.pop(), dt);


}

void World::draw() {
    if (PostEffect::isSupported()) {
        _sceneTextures[0]->clear();
        _sceneTextures[1]->clear();

        _sceneTextures[0]->setView(_worldView);
        _sceneTextures[1]->setView(_worldView);

        _gradientEffect.apply(*_sceneTextures[1], *_sceneTextures[0]);
        _sceneTextures[0]->draw(*_sceneLayers[SkyLayer]);
        _sceneTextures[0]->display();
        _bloomEffect.apply(*_sceneTextures[0], _target);

        for (int i = TreeBackLayer; i < LayerCount; i++)
            _target.draw(*_sceneLayers[i]);

        // _sceneTextures[0]->display();
        //_bloomEffect.apply(*_sceneTextures[1], _target);
    } else {
        // _target.setView(_worldView);
        _target.draw(_sceneGraph);
    }
}

CommandQueue& World::getCommandQueue() {
    return _commandQueue;
}

bool World::isOffLeft(const SceneNode &node) {
    sf::FloatRect nodeBounds(node.getBoundingRect());
    return (nodeBounds.left + nodeBounds.width < _worldBounds.left);
}


void World::spawnTrees(SceneNode &node, Category::Type type, const std::vector<TreeData> &data, int n, float x, float y, float width, float height) {
    for (int i = 0; i < n; i++) {
        int j = rand() % data.size();
        int u = rand() % int(width);
        int v = rand() % int(height);
        auto td = data[j];

        float scaleX = td.scaleX + ((rand() % 11 - 5) / 100.0);
        float scaleY = td.scaleY + ((rand() % 11 - 5) / 100.0);

        sf::Sprite sprite(_textures.get(td.texture), td.textureRect);
        sprite.scale(scaleX, scaleY);
        sf::FloatRect bounds = sprite.getGlobalBounds();
        sprite.setOrigin(bounds.width / 2.f, bounds.height);
        sprite.move(x + u, y + v);
        std::unique_ptr<SpriteNode> child(new SpriteNode(sprite, type));

        node.attachChild(std::move(child));
    }
}
