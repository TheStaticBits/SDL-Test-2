#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <nlohmann/json.hpp>

#include "vector.h"
#include "window.h"

enum ObjType { Platform_T, SilverStorage_T};
inline std::unordered_map<ObjType, std::string> objTNames = {
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
    Interactable(const Vect<uint32_t> tileSize, const std::vector<uint8_t> color, const ObjType type);
    Interactable(const ObjType type); // For loading from save
    virtual ~Interactable();

    void operator=(const Interactable&) = delete;

    virtual bool canPlace(const Vect<int64_t>& pos, std::vector<std::unique_ptr<Interactable>>& objects, const Vect<uint32_t>& size);
    virtual void completePlace(const uint64_t& time);

    virtual void update(const uint64_t& seconds) { };
    virtual void checkMenu(Window& window, const Vect<int64_t>& renderOffset);
    virtual void render(Window& window, const Vect<int64_t>& renderOffset);
    virtual void renderMenu(Window& window, const Vect<int64_t>& renderOffset);

    // Save functions
    virtual std::string getSave();
    virtual std::string readSave(std::string& save);
    
    // Getter functions
    inline bool isPlacing() const        { return placing;   }
    inline Vect<uint32_t>& getTileSize() { return tileSize;  }
    inline SDL_Rect& getRect()           { return renderPos; }
    inline const ObjType& getType()      { return type;      }
    inline const bool menuOpen()      { return hovering;  }

    // Setter functions
    inline void setPlacable(bool canPlace) { placable = canPlace; }
    inline void setPos(Vect<int64_t> pos)  { renderPos.x = pos.x; 
                                             renderPos.y = pos.y; }
    inline void removeMenu()               { hovering = false;
                                             clicked = false;     }

protected:
    static constexpr uint8_t alpha = 150;

    // SDL_Texture* image; // Add image later
    Vect<uint32_t> tileSize; // Amount of tiles it takes up
    std::vector<uint8_t> renderColor;
    SDL_Rect renderPos; // Top left corner

    bool placing;
    bool placable;

    bool hovering; // If menu is open or not
    bool clicked;
    Vect<uint32_t> menuSize;
    SDL_Rect menuPos;
    void setMenuRect(const Window& window, const Vect<int64_t>& renderOffset);

    const ObjType type;
};