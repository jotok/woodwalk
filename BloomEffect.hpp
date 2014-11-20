#pragma once

#include "PostEffect.hpp"
#include "Resources.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

#include <array>

class BloomEffect : public PostEffect {
    public:
        BloomEffect(ShaderHolder& shaders);
        virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) override;

    private:
        typedef std::array<sf::RenderTexture, 2> RenderTextureArray;

    private:
        void prepareTextures(sf::Vector2u size);

        void filterBright(const sf::RenderTexture& input, sf::RenderTexture& output);
        void blurMultipass(RenderTextureArray& renderTextures);
        void blur(const sf::RenderTexture& input, sf::RenderTexture& output, sf::Vector2f offsetFactor);
        void downsample(const sf::RenderTexture& input, sf::RenderTexture& output);
        void add(const sf::RenderTexture& source, const sf::RenderTexture& bloom, sf::RenderTarget& target);

    private:
        ShaderHolder& _shaders;

        sf::RenderTexture _brightnessTexture;
        RenderTextureArray _firstPassTextures;
        RenderTextureArray _secondPassTextures;
};