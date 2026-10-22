#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "../src/Defs.h"
#include "../src/World/Entity.h"
#include "../src/World/Formation.h"
#include "../src/World/Invader.h"
#include "../src/World/World.h"

TEST(Formation, FormationColumnSizeIsCorrect)
{
    json waveData = json::object({
        { "type", "invaders" },
        {
            "invaders",
            json::array({
                json::array({ "A", "A", "A", "A" }),
                json::array({ "B", "B", "B", "B" }),
                json::array({ "C", "C", "C", "C" }),
            }),
        },
    });

    World world = World{};
    Formation *formation = world.GetFormation();
    formation->CreateWave(waveData);

    EXPECT_EQ(formation->GetNumColumns(), 3);
}

TEST(Formation, FormationDXStepMovement)
{
    json waveData = json::object({
        { "type", "invaders" },
        {
            "invaders",
            json::array({
                json::array({ "A", "A", "A", "A" }),
                json::array({ "B", "B", "B", "B" }),
                json::array({ "C", "C", "C", "C" }),
            }),
        },
    });

    World world = World{};
    Formation *formation = world.GetFormation();
    formation->CreateWave(waveData);

    const Invader *invader = formation->GetColumn(0)->GetLowestInvader();

    float startX = invader->GetX();
    Vector2<float> startDir = invader->GetDir();
    formation->StepFormation();
    float finalX = invader->GetX();
    Vector2<float> finalDir = invader->GetDir();

    EXPECT_EQ(startDir * -1, finalDir);
    EXPECT_EQ(finalX - startX, FORMATION_DX_STEP);
}