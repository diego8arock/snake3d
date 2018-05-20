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
        
	typedef enum PartType {
                HEAD,
                SPINE,
                TAIL
        }PartType;
        
private:
        /**
         * Indica la cantidad de vertebras que tiene la serpiente.
         */
        static const int CANT_VERTEBS_SNAKE=10;
        
        /**
         * Variable que mantiene los indices de las vertebras.
         */
        static int VERTEB_INDEX;
        
        /**
         * Indice de la vertebra donde se quiere posicionar la camara.
         */
        static const int VERTEB_CAM_NODE_INDEX=3;

        /**
         * Constructor.
         * OJO debe ser privado. La serpiente es un Singleton.
         * Usar el metodo getSingleton() para obtener el puntero.
         */
        Snake(Ogre::SceneManager* scnMgr, PartType partType);
        
        /**
         * Destructor.
         */
        virtual ~Snake();
public:
        
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
	
	/**
	 * Modifica la dificultad
	 */
	void setDificulty(int dificulty);
        
        /**
         * Retorna el nodo asociado.
         */
        Ogre::SceneNode* getOgreNode();
        
        /**
         * Retorna el nodo donde se quiere posicionar la camara.
         */
        Ogre::SceneNode* getOgreCameraNode();
        
        /**
         * Retorna el singleton a la instancia de la serpiente (cabeza).
         */
        static Snake* getSingleton(Ogre::SceneManager* scnMgr);
        
        /**
         * Destruye el singleton.
         */
        static void destroySingleton();
        
protected:
        
        /**
         * Mantiene la instancia del singleton.
         */
        static Snake* mInstance;
                
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
         * Indica si es la ultima vertebra.
         */
        bool mIsTail;
        
        /**
         * SceneManager usado para crear los nodos que conforman la serpiente.
         */
        Ogre::SceneManager* mSceneMgr;
        
        /**
         * Nodo de ogre asociado a la vertebra.
         */
        Ogre::SceneNode* mNode;
        
        /**
         * Posicion actual de la vertebra de la vertebra que representa esta instancia.
         */
        Ogre::Vector3 mCurrentPos;
        
        /**
         * Posicion que va a tener la vertebra la siguiente vez que se llame al metodo draw.
         */
        Ogre::Vector3 mNextPosition;
        
        /**
         * Siguiente movimiento a realizar luego del draw.
         */
        NextMove mNextMove;
};

}//end namespace

#endif
