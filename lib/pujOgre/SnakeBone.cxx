#include <pujOgre/SnakeBone.h>

pujOgre::SnakeBone::SnakeBone()
{
  
}

pujOgre::SnakeBone::SnakeBone(std::string meshFileName,std::string meshName, Ogre::SceneManager* sceneMgr)
{
  Ogre::Entity* snakeBone =
    sceneMgr->createEntity(
      meshName, meshFileName
      );
  this->snakeBone = snakeBone; 
}

Ogre::Entity* pujOgre::SnakeBone::getEntity()
{
  return this->SnakeBone;
}


void pujOgre::SnakeBone::collition(Ogre::Entity objCollider)
{

}
void pujOgre::SnakeBone::destroy()
{

}
