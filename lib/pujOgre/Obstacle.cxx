#include <pujOgre/Obstacle.h>
#include <OgreSceneManager.h>
pujOgre::Obstacle::Obstacle()
{
  this->name="";
}

pujOgre::Obstacle::Obstacle(std::string name)
{
  this->name=name;
}

pujOgre::Obstacle::Obstacle(std::__cxx11::string name, std::__cxx11::string meshFileName,std::string meshName, Ogre::SceneManager* sceneMgr)
{
  this->name = name;
  Ogre::Entity* obstacle =
    sceneMgr->createEntity(
      meshName, meshFileName
      );
  this->obstacle = obstacle;
}

Ogre::Entity* pujOgre::Obstacle::getEntity()
{
  return this->obstacle;
}


void pujOgre::Obstacle::collition(Ogre::Entity objCollider)
{

}
void pujOgre::Obstacle::destroy()
{

}
