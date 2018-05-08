#ifndef _SNAKE_H__
#define _SNAKE_H__

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreVector3.h>
#include <string>

using namespace std;

namespace snake3d {

/**
 * Clase que representa la serpiente del juego.
 * Se trata de una lista enlazada nodos.
 */
class Snake 
{
public:
        typedef enum NextMove {
                NONE,
                RIGHT,
                LEFT,
                UP,
                DOWN
        }NextMove;
        
        /**
         * Constructor.
         */
        Snake(Ogre::SceneManager* scnMgr, bool isHead=false);
        
        /**
         * Destructor.
         */
        virtual ~Snake();
        
        /**
         * Agrega una nueva vertebra a la serpiente.
         */
        void addNewVerteb();
        
        /**
         * Direccionar la serpiente a la derecha (RIGHT-KEY)
         */
        void moveRigth();
        
        /**
         * Direccionar la serpiente a la izquierda (LEFT-KEY)
         */
        void moveLeft();
        
        /**
         * Direccionar la serpiente hacia arriba (UP-KEY)
         */
        void moveUp();
        
        /**
         * Direccionar la serpiente hacia abajo (DOWN-KEY).
         */
        void moveDown();
        
        /**
         * Dibuja la vertebra de la serpiente.
         * Es un metodo recursivo que dibuja la entidad en si misma
         * y a su vez dibuja la siguiente vertebra.
         */
        void draw();
        
protected:
        /**
         * Puntero a la siguiente vertebra.
         */
        Snake* mNextVerteb;
        
        /**
         * Nombre que se le asigna al nodo en el SceneManager.
         */
        string mName;
        
        /**
         * Indica si es la primera vertebra.
         */
        bool mIsHead;
        
        /**
         * SceneManager usado para crear los nodos que conforman la serpiente.
         */
        Ogre::SceneManager* mSceneMgr;
        
        Ogre::SceneNode* mNode;
        
        /**
         * Posicion actual de la vertebra de la vertebra que representa esta instancia.
         */
        Ogre::Vector3 mCurrentPos;
        
        /**
         * Posicion que va a tener la vertebra la siguiente vez que se llame al metodo draw.
         */
        Ogre::Vector3 mNextPosition;
        
        NextMove mNextMove;
};

}//end namespace

#endif
