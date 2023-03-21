#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "miniaudio.h"

#include "ecs/entity.h"
#include "components/transform.h"
#include "components/sprite.h"
#include "components/event_listener.h"

#include "renderer/sprite_renderer.h"
#include "utils/resource_manager.h"
#include "renderer/text_renderer.h"

class Game{
public:
    // game state
    bool                    Keys[1024];
    unsigned int            Width, Height;
    glm::vec2*              MousePos;
    bool                    MouseLeft, MouseRight;
    bool                    wireframe;
    Entity*                 root;
    bool                    played = false;
    //audio
    ma_engine               engine;
    ma_result               result;
    ma_sound                sound;
    
    // constructor/destructor
    Game(unsigned int width, unsigned int height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};

#endif
