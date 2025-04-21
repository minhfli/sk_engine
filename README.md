# SK(y) engine

sk engine is a simple 2d game framework using sdl and opengl written my MinhFli (me)

## Third party library used:

    SDL2:               for window and opengl
    GLAD:               for opengl
    STB_IMAGE:          load .png files
    NLOHMANN_JSON:      load .json files
    GLM:                math library
    ENTT:               ECS # currently not used

## To run game:

make your game folder in src/game/{game_src}

make a cpp file, include game.h and define all game.h's functions's logic

    # compile and run: (if you dont provide game's name, it will use GAME as name)
    make all GAME=game_src NAME=your_game_name

## sk_engine's modules:

### graphics:

simple opengl 2d renderer, can draw textured quad and line

#### window:

simple sdl wrapper for windowing

### physic2d:

#### pixel perfect:

2d physic implementation, based on celeste and towerdall physic: https://www.mattmakesgames.com/articles/celeste_and_towerfall_physics/index.html, for narrow phase collision detection (src/sk_engine/Physics/AABB_World_resolve.cpp)

Quad tree for broad phase collision detection (src/sk_engine/Physics/AABB_QuadTree.cpp)

#### box2d: (in development)

wrapper of box2d

### audio:

simple sdl_mixxer wrapper for audio

### common:

timer

readfile

handle error

## contact

vuvanminh.bacninh@gmail.com
