#ifndef __GWORLD_H__
#define __GWORLD_H__
#include <unordered_set>
#include <memory>

class GScene;
class GWorld
{
public:
    GWorld();
    virtual ~GWorld();

    void RunWithScene(std::shared_ptr<GScene> s);
};

#endif
