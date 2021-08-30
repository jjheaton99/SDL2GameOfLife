#pragma once

#include "SDL.h"
#include "GameObject.h"
#include <vector>
#include <memory>

//manages the creation, drawing, updating and destruction of objects
class GameObjectManager
{
private:

public:
    GameObjectManager();
    ~GameObjectManager();

    void update(const Camera& camera);
    void cameraDraw(const Camera& camera) const;
};

