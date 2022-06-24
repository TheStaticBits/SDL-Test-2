#include "button.h"

#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "window.h"
#include "vector.h"
#include "utility.h"
#include "animation.h"

// Empty JSON
nlohmann::json Button::buttonData = nlohmann::json(nlohmann::json::value_t::object);
std::unordered_map<std::string, SDL_Texture*> Button::textures = {};

Button::Button(Window& window, std::string texType,
               Vect<int64_t> pos, std::string text,
               const uint32_t fontSize, SDL_Color textColor)
    : texType(texType), 
      animation(textures[texType], 
                buttonData[texType]["anim"]["frames"].get<uint32_t>(), 
                buttonData[texType]["anim"]["delay"].get<float>()),
      size(animation.getFrame()),
      textImg(NULL), pos(pos), rect(util::getRect(pos, size)),
      hovering(false), pressed(false), activated(false)
{
    if (text != "")
    {
        TTF_Font* font = window.font(fontSize);
        textImg = window.getTextImg(font, text, textColor);
        TTF_CloseFont(font);

        textSize = util::getSize(textImg);
    }
    
    animation.lock();
}

Button::~Button()
{
    if (textImg != NULL) SDL_DestroyTexture(textImg);
    SDL_DestroyTexture(textures[texType]);
}

void Button::loadButtonData(Window& window)
{
    buttonData = nlohmann::json::parse(std::ifstream(DATA_PATH));

    // Load button textures
    for (const auto& data : buttonData.items())
        textures[data.key()] = window.scale(window.loadTexture(data.value()["path"].get<std::string>().c_str()));
}

void Button::resetTextures(Window& window)
{
    for (const auto& data : buttonData.items())
    {
        if (textures.find(data.key()) == textures.end()) 
            continue;

        SDL_Texture* tex = window.loadTexture(data.value()["path"].get<std::string>().c_str());
        SDL_Rect dst = util::getRect({0, 0}, util::getSize(tex) * SCALE);

        window.setTarget(textures[data.key()]);
        window.render(tex, dst);
        window.resetTarget();

        SDL_DestroyTexture(tex);
    }
}

void Button::setupText(std::string text, const uint32_t fontSize, SDL_Color color)
{

}

void Button::update(Window& window)
{
    updateRect();

    activated = false;
    hovering = util::collide(rect, window.getMousePos());
    
    animation.update(window);
    if (animation.isFinished())
    {
        animation.sendToOpposite();
        animation.lock();
    }
    
    if (hovering)
    {
        if (window.buttonHeld(SDL_BUTTON_LEFT))
        {
            if (!pressed) 
            {
                animation.resetDir(); // Runs only at first press
                animation.unlock();
            }

            pressed = true;
            window.setButton(SDL_BUTTON_LEFT, false); // So other elements below the button don't get activated
        }
        else if (pressed)
        {
            animation.reverse();
            animation.unlock();
            // Activates on button release
            pressed = false;
            activated = true;
        }
    }
    else if (pressed)
    {
        animation.reverse();
        animation.unlock();
        pressed = false;
    }
}

void Button::render(Window& window, Vect<int64_t> textOffset)
{
    // Modifying color
    if (hovering) 
    {
        // Might change to make this more efficient, like storing the
        // texture so SDL doesn't have to create another every frame.
        const Vect<uint32_t> size = animation.getFrame();
        
        SDL_Texture* tex = window.createTex(size.x, size.y);
        
        const nlohmann::json color = buttonData[texType]["hoverColorMod"];
        SDL_Rect rect = util::getRect({0, 0}, size);

        window.setTarget(tex);
        animation.render(window, {0, 0});
        window.drawRect(rect, {color[0], color[1], color[2], color[3]});
        window.resetTarget();

        SDL_Rect renderRect = util::getRect(pos, size);

        window.render(tex, renderRect);
        SDL_DestroyTexture(tex);
    }
    else
        animation.render(window, pos);

    // Draw text
    if (textImg != NULL)
    {
        SDL_Rect textRect = { pos.xCast<int>() + textOffset.xCast<int>() + 
                              (size.xCast<int>() - textSize.xCast<int>()) / 2,
                              pos.yCast<int>() + textOffset.yCast<int>() + 
                              (size.yCast<int>() - textSize.yCast<int>()) / 2, 

                              textSize.xCast<int>(), textSize.yCast<int>() };

        window.render(textImg, textRect);
    }
}

void Button::updateRect()
{
    rect = util::getRect(pos, size);
}