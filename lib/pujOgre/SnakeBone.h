#include <OgreEntity.h>

namespace Ogre
{
  class OverlaySystem;
}
namespace pujOgre{
  class SnakeBone{
    public:
      SnakeBone();
      SnakeBone(std::string meshFileName,std::string meshName, Ogre::SceneManager* sceneMgr);
    private:
      Ogre::Entity* snakeBone;
    public:
      Ogre::Entity* getEntity();
      void collition(Ogre::Entity objCollider);
      void destroy();
  };
}