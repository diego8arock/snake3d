#include <OgreEntity.h>

namespace Ogre
{
  class OverlaySystem;
}
namespace pujOgre{
class Obstacle{
public:
  Obstacle();
  Obstacle(std::string name);
  Obstacle(std::string name,std::string meshFileName,std::string meshName, Ogre::SceneManager* sceneMgr);
private:
  Ogre::Entity* obstacle;
  std::string name;
public:
  Ogre::Entity* getEntity();
  void collition(Ogre::Entity objCollider);
  void destroy();
};
}
