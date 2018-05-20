#include "Snake.h"
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <string>
#include <iostream>

namespace snake3d {

int Snake::VERTEB_INDEX=0;

//Inicializo instancia de singleton.
Snake* Snake::mInstance = NULL;

Snake* Snake::getSingleton(Ogre::SceneManager* scnMgr)
{
        if(Snake::mInstance == NULL) {
                Snake::mInstance = new Snake(scnMgr, PartType::HEAD);
                for(int i=1; i<CANT_VERTEBS_SNAKE; i++)
                        Snake::mInstance->addNewVerteb();
        }
        return Snake::mInstance;
}

void Snake::destroySingleton()
{
        if(Snake::mInstance != NULL)
                delete Snake::mInstance;
        
        Snake::mInstance = NULL;
}

Snake::Snake(Ogre::SceneManager* scnMgr, PartType partType)
{
        mSceneMgr = scnMgr;
        mNextVerteb = NULL;
        mIsTail = false;
        mIsHead = false;

        //Creo un nombre para este objeto de forma aleatoria.
        stringstream ss;
        ss.str();
        ss << "spine_";
        
        Ogre::Entity* entity = NULL;
	
	switch(partType)
	{
                case HEAD:
                        mName = "head";
                        entity = mSceneMgr->createEntity(mName,"snake_head.mesh");
                        mCurrentPos = Ogre::Vector3::UNIT_Y;
                        mIsHead = true;
                        break;
                case SPINE:
                        ss << VERTEB_INDEX;
                        VERTEB_INDEX++;
                        mName = ss.str();
                        entity = mSceneMgr->createEntity(mName,"snake_body.mesh");
                        mCurrentPos = Ogre::Vector3::ZERO;
                        break;
                case TAIL:
                        mName = "tail";
                        entity = mSceneMgr->createEntity(mName,"snake_tail.mesh");
                        mCurrentPos = Ogre::Vector3::ZERO;
                        mIsTail = true;
                        break;
	}
        
        entity->setCastShadows(true);
	
        mNode = mSceneMgr->getRootSceneNode( )->createChildSceneNode(mName);
        mNode->attachObject(entity);
        
	
        //Por defecto se ubica en el centro.
        //Ogre::AxisAlignedBox bbox = entity->getBoundingBox();
        //mCurrentPos = Ogre::Vector3(0,-bbox.getMinimum()[1],0);
        //mCurrentPos = Ogre::Vector3::UNIT_Y;
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

Ogre::SceneNode* Snake::getOgreCameraNode()
{
        Snake* n = this;
        int i=1;
        while(i < VERTEB_CAM_NODE_INDEX && n != NULL) {
                n = n->mNextVerteb;
                i++;
        }
        return n->mNode;
}

void Snake::addNewVerteb()
{
        //Si solo existe la cabeza o si la siguiente vertebra es la cola.
        if((mIsHead && mNextVerteb == NULL) || mNextVerteb->mIsTail) {
                
                //Si solo existe la cabeza genero una nueva vertebra de tipo "cola".
                if(mNextVerteb == NULL) {
                        mNextVerteb = new Snake(mSceneMgr, PartType::TAIL);
                        mNextVerteb->mCurrentPos = mCurrentPos+Ogre::Vector3::UNIT_Z;
                        mNextVerteb->mNextPosition = mNextVerteb->mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
                        mNextVerteb->mNode->translate(mNextVerteb->mCurrentPos);
                }
                //Si ya existe cabeza y cola. Genero una nueva "espina" y muevo la cola al final.
                else {
                        //Obteno el puntero a la cola para no perderlo.
                        Snake* tail = mNextVerteb;
                        Snake* spine = NULL;
                        std::cout << "Posicion de la cola antes de agregar espina: X:" << tail->mCurrentPos.x 
                        << " Y: " << tail->mCurrentPos.y << " Z: " << tail->mCurrentPos.z << std::endl;
                        
                        //Creo una nueva espina que va estar despues de la vertebra actual.
                        spine = new Snake(mSceneMgr, PartType::SPINE);
                        spine->mCurrentPos = mCurrentPos+Ogre::Vector3::UNIT_Z;
                        spine->mNextPosition = spine->mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
                        spine->mNode->translate(spine->mCurrentPos);
                        
                        std::cout << "Posicion de la nueva espina creada: X:" << spine->mCurrentPos.x 
                        << " Y: " << spine->mCurrentPos.y << " Z: " << spine->mCurrentPos.z << std::endl;
                        
                        //Ahora actualizo la posicion de la cola para que quede
                        //despues de la vertebra agregada en el paso anterior.
                        tail->mCurrentPos = spine->mCurrentPos+Ogre::Vector3::UNIT_Z;
                        tail->mNode->setPosition(spine->mNode->getPosition()+mCurrentPos+Ogre::Vector3::UNIT_Z);
                        tail->mNextPosition = tail->mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
                        //tail->mNode->translate(tail->mCurrentPos);
                        std::cout << "Posicion de la cola despues de agregar espina: X:" << tail->mCurrentPos.x 
                        << " Y: " << tail->mCurrentPos.y << " Z: " << tail->mCurrentPos.z << std::endl;
                        //Ahora indico que la espina creada tiene como siguiente vertebra la cola.
                        spine->mNextVerteb = tail;
                        
                        //Por ultimo la siguiente vertebra de la actual es la espina creada.
                        mNextVerteb = spine;
                }
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
                
                mNextVerteb->draw();
                
                //Aplico el movimiento de la vertebra actual a la siguiente.
                //Se hace luego del draw para que se haga efectivo en la siguiente escena.
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
        
        std::cout << "Nombre: " << mName << " Posicion Actual X:" << mCurrentPos.x 
                        << " Y: " << mCurrentPos.y << " Z: " << mCurrentPos.z << std::endl;
                        
        mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
}

void Snake::moveRigth()
{
        //mNode->resetOrientation();
        //mNode->yaw(Ogre::Degree(-5));
        //mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Y;
        mNode->rotate(Ogre::Quaternion(Ogre::Degree(-5), Ogre::Vector3::UNIT_Y));
        mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
        
        //Inyecto el movimiento a la siguiente vertebra. Se aplicara luego del draw. (delay 1)
        if(mNextVerteb)
                mNextVerteb->mNextMove = RIGHT; //Esto aplica luego del siguiente draw
}

void Snake::moveLeft()
{
        mNode->rotate(Ogre::Quaternion(Ogre::Degree(5), Ogre::Vector3::UNIT_Y));
        mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Z;
        //mNextPosition = mNode->getOrientation()*Ogre::Vector3::NEGATIVE_UNIT_Y;
        
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
