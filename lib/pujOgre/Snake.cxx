#include "Snake.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <string>

namespace snake3d {
  


Snake::Snake(Ogre::SceneManager* scnMgr, PartType partType)
{
        mSceneMgr = scnMgr;
        mNextVerteb = NULL;
        //Creo un nombre para este objeto de forma aleatoria.
        //TODO
        mName = "vert"+to_string(rand());
	//Nombre estático para la cabeza
	if(partType == snake3d::Snake::HEAD){
	  mName = "snake_head_node";
	}
        
        //TODO Cambiar el nombre del .mesh para el caso del cuerpo de la serpiente.
        Ogre::Entity* entity = NULL;
	
	switch(partType)
	{
	  case HEAD:
	    entity = mSceneMgr->createEntity(mName,"snake_head.mesh");
	    break;
	  case SPINE:
	    entity = mSceneMgr->createEntity(mName,"snake_body.mesh");
	    break;
	  case TAIL:
	    entity = mSceneMgr->createEntity(mName,"snake_tail.mesh");
	    break;
	}
        
        entity->setCastShadows( true );
	
        mNode = mSceneMgr->getRootSceneNode( )->createChildSceneNode(mName);
        mNode->attachObject(entity);
        
	
        //Por defecto se ubica en el centro.
        Ogre::AxisAlignedBox bbox = entity->getBoundingBox();
        mCurrentPos = Ogre::Vector3(0,-bbox.getMinimum()[1],0);
        //mCurrentPos = Ogre::Vector3(0,2,0);
        mNode->translate(mCurrentPos);
	mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
        mNextMove = NONE;
}

Snake::~Snake()
{
        if(mNextVerteb)
                delete mNextVerteb;
        
        delete mNode;
}

Ogre::SceneNode * Snake::getOgreNode()
{
        return mNode;
}

void Snake::addNewVerteb(PartType partType)
{
        if(mNextVerteb == NULL) {
                mNextVerteb = new Snake(mSceneMgr, partType);
                mNextVerteb->mCurrentPos = mCurrentPos+Ogre::Vector3::UNIT_Z;
                mNextVerteb->mNextPosition = mNextVerteb->mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
                mNextVerteb->mNode->translate(mNextVerteb->mCurrentPos);
        }else {
                mNextVerteb->addNewVerteb(partType);
        }
}

void Snake::draw()
{
        //Me dibujo a mi mismo
        mNode->translate(mNextPosition, Ogre::Node::TS_LOCAL);
        
        //Si existe una siguiente vertebra. La dibujo.
        if(mNextVerteb) {
                //mNextVerteb->mNextPosition = mCurrentPos;
                mNextVerteb->draw();
                switch(mNextVerteb->mNextMove)
                {
                        case RIGHT:
                                mNextVerteb->moveRigth();
                                break;
                        case LEFT:
                                mNextVerteb->moveLeft();
                                break;
			case UP:
				mNextVerteb->moveUp();
				break;
			case DOWN:
				mNextVerteb->moveDown();
				break;
                        default:
                                break;
                }
                mNextVerteb->mNextMove = NONE;
        }
        mCurrentPos = mNextPosition;
        mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
}

void Snake::moveRigth()
{
        mNode->rotate(Ogre::Quaternion(Ogre::Degree(-10), Ogre::Vector3::UNIT_Y));
        mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
        
        //Inyecto el movimiento a la siguiente vertebra. Se aplicara luego del draw. (delay 1)
        if(mNextVerteb)
                mNextVerteb->mNextMove = RIGHT; //Esto aplica luego del siguiente draw
}

void Snake::moveLeft()
{
        mNode->rotate(Ogre::Quaternion(Ogre::Degree(10), Ogre::Vector3::UNIT_Y));
	
        mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
        if(mNextVerteb)
                mNextVerteb->mNextMove = LEFT;
}

void Snake::moveUp()
{
  mNode->rotate(Ogre::Quaternion(Ogre::Degree(10), Ogre::Vector3::UNIT_Z));
  mNode->rotate(Ogre::Quaternion(Ogre::Degree(10), Ogre::Vector3::UNIT_X));
  
  mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
  if(mNextVerteb)
                mNextVerteb->mNextMove = UP;
}
void Snake::moveDown()
{
  mNode->rotate(Ogre::Quaternion(Ogre::Degree(-10), Ogre::Vector3::UNIT_Z));
  mNode->rotate(Ogre::Quaternion(Ogre::Degree(-10), Ogre::Vector3::UNIT_X));
  
  mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
  if(mNextVerteb)
                mNextVerteb->mNextMove = DOWN;
}


}//end namespace
