#pragma once

#include "Resources.hpp"

#include <SFML/Graphics/Rect.hpp>

#include <vector>

struct TreeData {
    TreeData(Textures texture, sf::IntRect textureRect, float scaleX=1.f, float scaleY=1.f);
    Textures texture;
    sf::IntRect textureRect;
    float scaleX;
    float scaleY;
};

struct TreeLayerData {
    TreeLayerData(float, float, double, float);
    double spawnRate(float, float) const;

    // define the spawn rectangle as a fraction of the view height
    float fy;
    float fheight;

    // spawn rate per 100x100 area
    double baseRate;

    // for parallax effect
    float speedMultiplier;

    std::vector<TreeData> trees;
};

void initTreeLayers(std::vector<TreeLayerData>&);
