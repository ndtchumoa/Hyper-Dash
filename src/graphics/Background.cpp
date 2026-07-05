#include "graphics/Background.h"

#include "systems/AssetManager.h"

bool Background::init(AssetManager& assets)
{
    layers.clear();

    const struct
    {
        const char* path;
        float speed;
    }
    configs[] =
    {
        {
            "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 1.png",
            0.2f
        },

        {
            "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 2.png",
            0.4f
        },

        {
            "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 3.png",
            0.7f
        },

        {
            "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 4.png",
            1.0f
        },

        {
            "assets/textures/background/Normal BG/GandalfHardcore Background layers_layer 5.png",
            1.4f
        }
    };

    for (const auto& cfg : configs)
    {
        Layer layer;

        layer.texture = assets.getTexture(cfg.path);

        if (!layer.texture)
            continue;

        layer.speed = cfg.speed;

        SDL_QueryTexture(
            layer.texture,
            nullptr,
            nullptr,
            &layer.width,
            &layer.height);

        layers.push_back(layer);
    }

    return !layers.empty();
}

void Background::update()
{
    for (auto& layer : layers)
    {
        layer.scroll += layer.speed;

        if (layer.scroll >= layer.width)
            layer.scroll = 0.0f;
    }
}

void Background::render(SDL_Renderer* renderer)
{
    for (auto& layer : layers)
    {
        SDL_Rect dst1 =
        {
            -(int)layer.scroll,
            0,
            layer.width,
            720
        };

        SDL_Rect dst2 =
        {
            dst1.x + layer.width,
            0,
            layer.width,
            720
        };

        SDL_RenderCopy(
            renderer,
            layer.texture,
            nullptr,
            &dst1);

        SDL_RenderCopy(
            renderer,
            layer.texture,
            nullptr,
            &dst2);
    }
}

void Background::clean()
{
    layers.clear();
}