#include <iostream>

#ifdef __EMSCRIPTEN__

#include <emscripten.h>

// For the web build (retrieves save from cookies)
EM_JS(void, setSave, (const char* saveName, const char* saveData), {
    document.cookie = UTF8ToString(saveName) + "=" + UTF8ToString(saveData) + ";";
});

EM_JS(char*, getSave, (const char* saveName), {
    var data = document.cookie.substring(UTF8ToString(saveName).length + 1, document.cookie.length);
    var lengthBytes = lengthBytesUTF8(data) + 1;
    var stringOnHeap = _malloc(lengthBytes);
    stringToUTF8(data, stringOnHeap, lengthBytes);
    return stringOnHeap;
});

#else

#include <fstream>

void setSave(std::string saveName, std::string saveData)
{
    std::ofstream file(saveName + ".txt");
    file << saveData;
    file.close();
}

// For the C++ build (retrieves from save file)
char* getSave(std::string saveName)
{
    std::ifstream file(saveName + ".txt");
    std::string data;
    if (file.is_open())
    {
        std::getline(file, data);
        file.close();
        return (char*)data.c_str();
    }
    else
        std::cout << "[Error] Failed to open save file: " << saveName << ".txt" << std::endl;
    
    return nullptr;
}

#endif