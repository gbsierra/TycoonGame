#include "Mine.h"
#include "../GameConstants.h"

Mine::Mine()
    : Building(BuildingType::MINE,
               "Mine",
               500.0f,                                                                                    // cost
               0.3f,                                                                                      // base production rate
               {Resource(ResourceType::ENERGY, "Energy", 0.0f, GameConstants::ENERGY_BASE_PRICE, false)}, // input resource
               {Resource(ResourceType::STONE, "Stone", 0.0f, GameConstants::STONE_BASE_PRICE, false),     // output resource
                Resource(ResourceType::IRON, "Iron", 0.0f, GameConstants::IRON_BASE_PRICE, false)},       // output resource
               GameConstants::MINE_MAINTENANCE,                                                           // maintenance cost
               250.0f,                                                                                    // upgrade cost
               10)                                                                                        // required reputation
{
}

void Mine::UpdateEfficiency()
{
    // First use the base class efficiency calculation
    Building::UpdateEfficiency();

    // Mines have additional efficiency requirements
    bool hasEnergy = false;
    for (const auto &resource : GetInputResources())
    {
        if (resource.GetAmount() > 0)
        {
            hasEnergy = true;
            break;
        }
    }

    // Apply additional efficiency factors based on resource availability
    float additionalEfficiency = hasEnergy ? 1.0f : 0.2f;

    // Combine base efficiency with additional factors
    SetEfficiency(GetEfficiency() * additionalEfficiency);
}

float Mine::CalculateProduction(float deltaTime) const
{
    // Mines have a chance to find rare resources based on level
    float baseProduction = Building::CalculateProduction(deltaTime);
    float rareChance = GetLevel() * 0.03f; // 3% chance per level for rare resources

    if (static_cast<float>(rand()) / RAND_MAX < rareChance)
    {
        return baseProduction * 2.0f; // Double production for rare finds
    }
    return baseProduction;
}