#pragma once

namespace sf {
    class Texture;
    class Font;
    class Shader;
}

enum class Textures {
    Trees
};

enum class Fonts {
    Title
};

enum class Shaders {
    BrightnessPass,
    DownSamplePass,
    GaussianBlurPass,
    AddPass,
    Gradient
};

template <typename K, typename V>
class ResourceHolder;

typedef ResourceHolder<Textures, sf::Texture> TextureHolder;
typedef ResourceHolder<Fonts, sf::Font> FontHolder;
typedef ResourceHolder<Shaders, sf::Shader> ShaderHolder;
