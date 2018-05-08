#include <pujOgre/Apple.h>

pujOgre::Apple::Apple()
{
  this->name="";
}

pujOgre::Apple::Apple(std::string name)
{
  this->name=name;
}

pujOgre::Apple::Apple(std::string name, std::string meshFileName,std::string meshName, Ogre::SceneManager* sceneMgr)
{
  this->name = name;
  Ogre::Entity* apple =
    sceneMgr->createEntity(
      meshName, meshFileName
      );
  this->apple = apple;
}

Ogre::Entity* pujOgre::Apple::getEntity()
{
  return this->apple;
}


void pujOgre::Apple::collition(Ogre::Entity objCollider)
{

}
void pujOgre::Apple::destroy()
{

}
