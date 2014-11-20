#pragma once

#include "PostEffect.hpp"
#include "Resources.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Shader.hpp>

class GradientEffect : public PostEffect {
    public:
        GradientEffect(ShaderHolder& shaders, sf::Color horizonColor, sf::Color zenithColor, float power = 1.0);
        virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) override;
        void setScale(float p);

    private:
        ShaderHolder& _shaders;
        sf::Color _horizonColor;
        sf::Color _zenithColor;

        sf::Texture _randomTexture;
        float _scale;
};
