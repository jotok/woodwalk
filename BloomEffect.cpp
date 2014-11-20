#include "BloomEffect.hpp"

#include <SFML/Graphics/Shader.hpp>

BloomEffect::BloomEffect(ShaderHolder& shaders)
    : _shaders{shaders}
    , _brightnessTexture{}
    , _firstPassTextures{}
    , _secondPassTextures{}
{}

void BloomEffect::apply(const sf::RenderTexture& input, sf::RenderTarget& output) {
    prepareTextures(input.getSize());

    filterBright(input, _brightnessTexture);

	downsample(_brightnessTexture, _firstPassTextures[0]);
	blurMultipass(_firstPassTextures);

    downsample(_firstPassTextures[0], _secondPassTextures[0]);
    blurMultipass(_secondPassTextures);

    add(_firstPassTextures[0], _secondPassTextures[0], _firstPassTextures[1]);
    _firstPassTextures[1].display();
    add(input, _firstPassTextures[1], output);
}

void BloomEffect::prepareTextures(sf::Vector2u size) {
    if (_brightnessTexture.getSize() != size) {
        _brightnessTexture.create(size.x, size.y);
        _brightnessTexture.setSmooth(true);

        _firstPassTextures[0].create(size.x / 2, size.y / 2);
        _firstPassTextures[0].setSmooth(true);
        _firstPassTextures[1].create(size.x / 2, size.y / 2);
        _firstPassTextures[1].setSmooth(true);

        _secondPassTextures[0].create(size.x / 4, size.y / 4);
        _secondPassTextures[0].setSmooth(true);
        _secondPassTextures[1].create(size.x / 4, size.y / 4);
        _secondPassTextures[1].setSmooth(true);
    }
}

void BloomEffect::filterBright(const sf::RenderTexture& input, sf::RenderTexture& output) {
    sf::Shader& brightness = _shaders.get(Shaders::BrightnessPass);

    brightness.setParameter("source", input.getTexture());
    applyShader(brightness, output);
    output.display();
}

void BloomEffect::blurMultipass(RenderTextureArray& renderTextures) {
    sf::Vector2u textureSize = renderTextures[0].getSize();

    for (std::size_t count = 0; count < 2; ++count) {
        blur(renderTextures[0], renderTextures[1], sf::Vector2f(0.f, 1.f / textureSize.y));
        blur(renderTextures[1], renderTextures[0], sf::Vector2f(1.f / textureSize.x, 0.f));
    }
}

void BloomEffect::blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor)
{
    sf::Shader& gaussianBlur = _shaders.get(Shaders::GaussianBlurPass);

    gaussianBlur.setParameter("source", input.getTexture());
    gaussianBlur.setParameter("offsetFactor", offsetFactor);
    applyShader(gaussianBlur, output);
    output.display();
}

void BloomEffect::downsample(const sf::RenderTexture& input, sf::RenderTexture& output) {
    sf::Shader& downSampler = _shaders.get(Shaders::DownSamplePass);

    downSampler.setParameter("source", input.getTexture());
    downSampler.setParameter("sourceSize", sf::Vector2f(input.getSize()));
    applyShader(downSampler, output);
    output.display();
}

void BloomEffect::add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& output)
{
    sf::Shader& adder = _shaders.get(Shaders::AddPass);

    adder.setParameter("source", source.getTexture());
    adder.setParameter("bloom", bloom.getTexture());
    applyShader(adder, output);
}
