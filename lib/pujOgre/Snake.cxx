#include "Snake.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <string>

namespace snake3d {

Snake::Snake(Ogre::SceneManager* scnMgr, bool isHead)
{
        mSceneMgr = scnMgr;
        mNextVerteb = NULL;
        mIsHead = isHead;
        //Creo un nombre para este objeto de forma aleatoria.
        mName = "vert"+to_string(rand());
        
        //TODO Cambiar el nombre del .mesh para el caso del cuerpo de la serpiente.
        Ogre::Entity* entity = NULL;
        entity = mIsHead ? mSceneMgr->createEntity(mName,"snake_head.mesh") : mSceneMgr->createEntity(mName,"snake_head.mesh");
        
        entity->setCastShadows( true );
        
        mNode = mSceneMgr->getRootSceneNode( )->createChildSceneNode(mName);
        mNode->attachObject(entity);
        
        //Por defecto se ubica en el centro.
        Ogre::AxisAlignedBox bbox = entity->getBoundingBox();
        //mCurrentPos = Ogre::Vector3(0,-bbox.getMinimum()[1],0);
        mCurrentPos = Ogre::Vector3(0,2,0);
        mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
        mNode->translate(mCurrentPos);
        mNextMove = NONE;
}

Snake::~Snake()
{
        if(mNextVerteb)
                delete mNextVerteb;
        
        delete mNode;
}

void Snake::addNewVerteb()
{
        if(mNextVerteb == NULL) {
                mNextVerteb = new Snake(mSceneMgr);
                mNextVerteb->mCurrentPos = mCurrentPos+Ogre::Vector3::UNIT_Z;
                mNextVerteb->mNextPosition = mNextVerteb->mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
                mNextVerteb->mNode->translate(mNextVerteb->mCurrentPos);
        }else {
                mNextVerteb->addNewVerteb();
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
        mNode->rotate(Ogre::Quaternion(Ogre::Degree(-15), Ogre::Vector3::UNIT_Y));
        mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
        
        //Inyecto el movimiento a la siguiente vertebra. Se aplicara luego del draw. (delay 1)
        if(mNextVerteb)
                mNextVerteb->mNextMove = RIGHT; //Esto aplica luego del siguiente draw
}

void Snake::moveLeft()
{
        mNode->rotate(Ogre::Quaternion(Ogre::Degree(15), Ogre::Vector3::UNIT_Y));
        mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
        if(mNextVerteb)
                mNextVerteb->mNextMove = LEFT;
}

}//end namespace
