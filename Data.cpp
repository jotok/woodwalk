#include "Data.hpp"

TreeData::TreeData(Textures texture, sf::IntRect textureRect, float scaleX, float scaleY)
: texture{texture}
, textureRect{textureRect}
, scaleX{scaleX}
, scaleY{scaleY}
{}

TreeLayerData::TreeLayerData(float fy, float fheight, double baseRate, float speedMultiplier)
: fy{fy}
, fheight{fheight}
, baseRate{baseRate}
, speedMultiplier{speedMultiplier}
{}

double TreeLayerData::spawnRate(float width, float height) const {
    return baseRate * width * height / (100.f * 100.f);
}
void initTreeLayers(std::vector<TreeLayerData>& data) {

    TreeLayerData backLayer(0.5, 0.2, 2, 0.8);
    backLayer.trees.emplace_back(Textures::Trees, sf::IntRect(92, 70, 518, 896), 0.50, 0.50);
    backLayer.trees.emplace_back(Textures::Trees, sf::IntRect(646, 88, 434, 606), 0.50, 0.50);
    backLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1130, 18, 384, 782), 0.50, 0.50);
    backLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1552, 56, 512, 716), 0.50, 0.50);
    backLayer.trees.emplace_back(Textures::Trees, sf::IntRect(32, 1032, 456, 640), 0.50, 0.50);
    backLayer.trees.emplace_back(Textures::Trees, sf::IntRect(492, 862, 434, 844), 0.50, 0.50);
    backLayer.trees.emplace_back(Textures::Trees, sf::IntRect(940, 804, 472, 834), 0.50, 0.50);
    backLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1450, 850, 668, 660), 0.50, 0.50);
    data.push_back(backLayer);

    TreeLayerData middleLayer(0.7, 0.2, 1.8, 1.1);
    middleLayer.trees.emplace_back(Textures::Trees, sf::IntRect(48, 2037, 606, 801), 0.45, 0.45);
    middleLayer.trees.emplace_back(Textures::Trees, sf::IntRect(597, 1671, 636, 615), 0.45, 0.45);
    middleLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1257, 1749, 225, 462), 0.45, 0.45);
    middleLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1521, 1590, 573, 612), 0.45, 0.45);
    middleLayer.trees.emplace_back(Textures::Trees, sf::IntRect(594, 2458, 216, 459), 0.45, 0.45);
    middleLayer.trees.emplace_back(Textures::Trees, sf::IntRect(876, 2280, 459, 585), 0.45, 0.45);
    middleLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1453, 2241, 582, 696), 0.45, 0.45);
    data.push_back(middleLayer);

    TreeLayerData frontLayer(0.9, 0.1, 2.1, 1.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(68, 2930, 204, 200), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(68, 3202, 250, 130), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(418, 2932, 294, 316), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(750, 2966, 418, 160), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1220, 2972, 308, 234), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1532, 3176, 164, 130), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1722, 3032, 404, 292), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(42, 3326, 592, 236), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(690, 3238, 502, 274), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1320, 3368, 324, 144), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1756, 3372, 314, 140), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(85, 3642, 613, 189), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(818, 3624, 423, 183), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1301, 3579, 370, 260), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1791, 3530, 284, 451), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(35, 3881, 856, 348), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(974, 3899, 234, 282), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1290, 4014, 326, 177), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1664, 4047, 423, 269), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(89, 4389, 474, 170), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(684, 4247, 429, 358), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1170, 4263, 366, 339), 0.5, 0.5);
    frontLayer.trees.emplace_back(Textures::Trees, sf::IntRect(1614, 4364, 501, 273), 0.5, 0.5);
    data.push_back(frontLayer);
}
