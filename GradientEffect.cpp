#include "GradientEffect.hpp"

#include <SFML/Graphics/VertexArray.hpp>

#include <cstdlib>

GradientEffect::GradientEffect(ShaderHolder& shaders, sf::Color horizonColor, sf::Color zenithColor, float scale)
    : _shaders{shaders}
    , _horizonColor{horizonColor}
    , _zenithColor{zenithColor}
    , _randomTexture{}
    , _scale{scale}
{
    _randomTexture.setRepeated(false);
}

void GradientEffect::setScale(float scale) {
    _scale = scale;
}

void GradientEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output) {

    sf::Vector2u size = input.getSize();

    if (_randomTexture.getSize() != size) {
        sf::Image randomImage;
        randomImage.create(size.x, size.y);

        for (size_t x = 0; x < size.x; x++) {
            for (size_t y = 0; y < size.y; y++) {
                randomImage.setPixel(x, y, sf::Color(rand() % 255, rand() % 255, rand() % 255));
            }
        }

        _randomTexture.loadFromImage(randomImage);
    }

    sf::Vector2f scale = {_scale, _scale};

    sf::Shader& gradient = _shaders.get(Shaders::Gradient);
    gradient.setParameter("windowSize", size.x, size.y);
    gradient.setParameter("horizonColour", _horizonColor);
    gradient.setParameter("zenithColour", _zenithColor);
    gradient.setParameter("randomTexture", _randomTexture);
    gradient.setParameter("scale", scale);

    applyShader(gradient, output);
}
