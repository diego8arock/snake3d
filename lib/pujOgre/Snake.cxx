#include <pujOgre/Snake.h>
#include <list>

pujOgre::Snake::Snake()
{

}

pujOgre::Snake::Snake(std::string meshFileNameHead, std::string meshNameHead, Ogre::SceneManager* sceneMgr, int initialSnakeBone)
{
  Ogre::Entity* head =
    sceneMgr->createEntity(
      meshNameHead, meshFileNameHead
      );
  this->head = head;
  setInitialSnakeBone(initialSnakeBone);
}

void pujOgre::Snake::setInitialSnakeBone(int initialSnakeBone)
{
    
}

void pujOgre::Snake::appendSnakeBone(pujOgre::SnakeBone snakeBone)
{

}

void pujOgre::Snake::destroy()
{

}


