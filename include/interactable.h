#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

class Window;
#include "vector.h"
#include "animation.h"

enum ObjType { Platform_T, SilverStorage_T};
inline const std::unordered_map<ObjType, const std::string> objTNames = {
    { Platform_T,      "Platform"      },
    { SilverStorage_T, "Silver Storage"}
};

// If the first of a string is the same name as the object
inline bool objCheckSavePart(const std::string part, const ObjType objType) 
{ 
    return part.substr(0, objTNames.at(objType).length()) == objTNames.at(objType); 
}

// Objects in the home base that can be interacted with
class Interactable
{
public:
    Interactable(Window& window, const nlohmann::json& data, const Vect<uint32_t> tileSize, const ObjType type);
    Interactable(Window& window, const nlohmann::json& data, const ObjType type); // For loading from save
    virtual ~Interactable();

    void operator=(const Interactable&) = delete;

    void loadImgs(Window& window, const nlohmann::json& data);
    void setupAnims(Window& window, const nlohmann::json& data);

    virtual bool canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size);
    virtual void completePlace(const uint64_t& time);

    virtual void update(Window& window, const uint64_t& time);
    virtual void checkMenu(Window& window, const Vect<int64_t>& renderOffset, const Vect<uint32_t> baseSize);
    virtual void render(Window& window, const Vect<int64_t>& renderOffset);
    virtual void renderMenu(Window& window, const Vect<int64_t>& renderOffset);
    
    void updateAnim(Window& window);
    void setModColor(Window& window);
    void swapAnim(const std::string& name);

    // Save functions
    virtual std::string getSave();
    virtual std::string readSave(std::string& save);
    
    // Getter functions
    inline bool isPlacing() const        { return placing;   }
    inline Vect<uint32_t>& getTileSize() { return tileSize;  }
    inline SDL_Rect& getRect()           { return renderPos; } 
    inline const ObjType& getType()      { return type;      }
    inline const bool menuOpen()         { return hovering;  }
    
    inline const Vect<int> getCenter()  { return { renderPos.x + renderPos.w, 
                                                   renderPos.y + renderPos.h }; }

    inline const nlohmann::json& getAnimData(const nlohmann::json& data, 
                                             const std::string& anim, 
                                             const std::string& property) const 
    { 
        return data["animData"][anim][property]; 
    }

    // Setter functions
    inline void setPlacable(bool canPlace) { placable = canPlace; }
    inline void setPos(Vect<int64_t> pos)  { renderPos.x = pos.x; 
                                             renderPos.y = pos.y; }
    inline void removeMenu()               { hovering = false;
                                             clicked = false;     }

protected:
    void setMenuRect(const Window& window, const Vect<int64_t>& renderOffset, const Vect<uint32_t> baseSize);
    
    static constexpr uint8_t ALPHA = 150;
    static std::unordered_map<ObjType, std::unordered_map<std::string, SDL_Texture*>> textures;

    std::unordered_map<std::string, std::unique_ptr<Animation>> anims;
    std::string currentAnim;
    Vect<uint32_t> tileSize; // Amount of tiles the hitbox/collision takes up
    std::vector<uint8_t> modColor;
    SDL_Rect renderPos; // Top left corner

    bool placing;
    bool placable;

    bool hovering; // If menu is open or not
    bool clicked;
    Vect<uint32_t> menuSize;
    SDL_Rect menuPos;

    const ObjType type;
};