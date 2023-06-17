#pragma once

#include <cstdint>

// entity tag, entity can have multiple tag
enum ptag : uint64_t {

    // for physic
    PHY_SOLID,
    PHY_ACTOR,
    PHY_TRIGGER,
    PHY_ONE_WAY, //should be solid, only work in upward direction(for now)

    PHY_DIR_U, // for one way body, this can change how trigger work, dont use for entity 
    PHY_DIR_D,
    PHY_DIR_L,
    PHY_DIR_R,

    PHY_MOVEABLE, // only physic body with this tag will be query and updated, all actor is moveable

};
inline bool CheckTag(uint64_t i, int tag) {
    return ((i >> tag) & 1) != 0;
}
inline uint64_t AddTag(uint64_t& i, int tag) {
    i += (1ll << tag);
}