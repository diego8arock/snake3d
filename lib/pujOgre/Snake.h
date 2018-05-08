#include <OgreEntity.h>
#include "SnakeBone.h"
#include <list>

namespace Ogre
{
  class OverlaySystem;
}
namespace pujOgre{
  class Snake{
  public:
    Snake();
    Snake(std::string meshFileNameHead,std::string meshNameHead, Ogre::SceneManager* sceneMgr, int initialSnakeBone);
  private:
    Ogre::Entity* head;
    std::list<SnakeBone> body;
  public:
    void setInitialSnakeBone(int initialSankeBone);
    void appendSnakeBone(SnakeBone snakeBone);
    void destroy();
  };
}