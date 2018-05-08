#include <OgreEntity.h>

namespace Ogre
{
  class OverlaySystem;
}
namespace pujOgre{
class Apple {
public:
  Apple();
  Apple(std::string name);
  Apple(std::string name,std::string meshFileName,std::string meshName, Ogre::SceneManager* sceneMgr);
private:
  Ogre::Entity* apple;
  std::string name;
  int score;
public:
  Ogre::Entity* getEntity();
  void collition(Ogre::Entity objCollider);
  void destroy();
};
}