#include <iostream>
#include <pujOgre/Application.h>
#include <pujOgre/Player.h>
#include <pujOgre/Apple.h>
#include <pujOgre/Level.h>
#include <pujOgre/Obstacle.h>
#include <pujOgre/Snake.h>

#include <OgreBone.h>
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreKeyFrame.h>
#include <OgreMeshManager.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreSkeletonInstance.h>
#include <OgreVector3.h>
#include <OgreLogManager.h>
#include <OgreStringConverter.h>
#include <OgreRay.h>

/**
 * Clase principal del juego Snake3d.
 */
class Snake3d : public pujOgre::Application
{
public:
        Snake3d( );
        virtual ~Snake3d( );

protected:
        virtual void createCamera( ) override;
        virtual void createScene( ) override;
        virtual bool frameRenderingQueued( const Ogre::FrameEvent& evt ) override;
        virtual bool keyPressed( const OIS::KeyEvent& arg ) override;
        virtual bool keyReleased( const OIS::KeyEvent& arg ) override;
        void createAnimations( );

protected:
        pujOgre::Player* mPlayer;
        Ogre::AnimationState* m_AnimationState;
        snake3d::Snake* mSnake;

        bool mRightPress = false;
        bool mLeftPress = false;
        bool mUpPress = false;
        bool mDownPress = false;
        
        int timeDelay =0;
        int timeDelayMax = 150;
        float snake_velocity = 1;
        int game_difficulty = 0;
        
        //Movement
        float degree_right=-5;
        float degree_left=5;
        int num_apple_allow = 10;
        int num_apple_inScene = 0;
        float mesh_apple_lenght = 0;
        
        //Temporal
        int mContador=0;
};

// -------------------------------------------------------------------------
// Main
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus
  
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    int main( int argc, char* argv[] )
#endif // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  {
    // Create application object
    Snake3d app;
    try
    {
      app.go( );
    }
    catch( Ogre::Exception& e )
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      std::cerr
        << "An exception has occured: "
        << e.getFullDescription( ).c_str( )
        << std::endl;
#endif // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    } // yrt
    return( 0 );
  }
#ifdef __cplusplus
}
#endif // __cplusplus

// -------------------------------------------------------------------------
Snake3d::Snake3d() : pujOgre::Application( )
{
        //TODO Corregir
        if(game_difficulty==0){
                snake_velocity = 1;
                timeDelayMax = 2;
                degree_right = -10;
                degree_left = 10;
        }else{
                if(game_difficulty==1){
                        snake_velocity = 1;
                        timeDelayMax = 9;
                        degree_right = -15;
                        degree_left = 15;
                }
        }
}

// -------------------------------------------------------------------------
Snake3d::~Snake3d( )
{
}

// -------------------------------------------------------------------------
void Snake3d::createCamera( )
{
        this->pujOgre::Application::createCamera( );
        this->m_Camera->setPosition( Ogre::Vector3( 0, 0, 0 ) );
        this->m_Camera->setNearClipDistance( 5 );
}

// -------------------------------------------------------------------------
void Snake3d::createScene( )
{
        // Lights
        this->m_SceneMgr->setAmbientLight( Ogre::ColourValue( 1, 1, 1 ) );
        this->m_SceneMgr->setShadowTechnique( Ogre::SHADOWTYPE_STENCIL_ADDITIVE );

        Ogre::Light* light1 = this->m_SceneMgr->createLight( "Light1" );
        light1->setType( Ogre::Light::LT_POINT );
        light1->setPosition( Ogre::Vector3( 50, 50, 50 ) );
        light1->setDiffuseColour( 1.0, 1.0, 1.0 );
        light1->setSpecularColour( 1.0, 1.0, 1.0 );

        Ogre::Light* light2 = this->m_SceneMgr->createLight( "Light2" );
        light2->setType( Ogre::Light::LT_POINT );
        light2->setPosition( Ogre::Vector3( 0, 50, -50 ) );
        light2->setDiffuseColour( 0.5, 0.5, 0.5 );
        light2->setSpecularColour( 0.5, 0.5, 0.5 );

        // Create a plane
        Ogre::Plane plane( Ogre::Vector3::UNIT_Y, 0 );
        Ogre::MeshManager::getSingleton( ).createPlane(
                "plane",
                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                plane,
                150, 150, 20, 20, true, 1, 5, 5,
                Ogre::Vector3::UNIT_Z
        );

        // Associate a floor entity with the created plane
        Ogre::Entity* floor = this->m_SceneMgr->createEntity( "floor", "plane" );
        floor->setMaterialName( "Mat" );
        this->m_SceneMgr->getRootSceneNode( )->attachObject( floor );

        // Load model entity
        
        Ogre::Entity* apple = this->m_SceneMgr->createEntity(
                "apple", "apple.mesh"
        );
        apple->setCastShadows( true );
        Ogre::AxisAlignedBox apple_bbox = apple->getBoundingBox( );
        
        mesh_apple_lenght = apple_bbox.getMaximum().x-apple_bbox.getMinimum().x;
        
        Ogre::SceneNode* apple_node = this->m_SceneMgr->getRootSceneNode( )->createChildSceneNode(
        "apple_node"
        );
        
        num_apple_inScene = 1;
        apple_node->scale(Ogre::Vector3(0.5,0.5,0.5));
        apple_node->attachObject( apple );
        //   apple_node->translate( 0, -bbox.getMinimum( )[ 1 ], -20 );

        mSnake = snake3d::Snake::getSingleton(m_SceneMgr);
        
        Ogre::SceneNode* snake_cam_node =
        this->m_SceneMgr->getSceneNode("MainCamNode");
        // GETTING HEAD NODE TO ATTACH CAMERA
        
        Ogre::SceneNode* snake_head_node = mSnake->getOgreNode();
        
        Ogre::Vector3 direction = snake_head_node->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
        Ogre::Vector3 pos = snake_head_node->getPosition();
        
        direction = snake_head_node->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;

        //INITIAL CAM POSITION FPS CAM
        this->m_Camera->setPosition(Ogre::Vector3(pos.x,0,pos.z));
        
        //UNCOMMET OR COMMENT FOR HAVE A PERSPECTIVE VIEW
        this->m_Camera->setPosition(Ogre::Vector3(pos.x + 30,10,pos.z-30));
        
        //UNCOMMET FOR FPS CAM OR COMMENT FOR HAVE A PERSPECTIVE VIEW
        //snake_cam_node->detachObject(this->m_Camera);
        //snake_head_node->attachObject(this->m_Camera);
        
        
        /* // Prepare skeleton to be manually controlled
        Ogre::SkeletonInstance* snake_head_skel = snake_head->getSkeleton( );
        for( unsigned int bIt = 0; bIt < snake_head_skel->getNumBones( ); bIt++ )
        snake_head_skel->getBone( bIt )->setManuallyControlled( true );*/
}

// -------------------------------------------------------------------------
bool Snake3d::frameRenderingQueued( const Ogre::FrameEvent& evt )
{
        //Nodes in scene
        Ogre::SceneNode* snake_head_node = this->mSnake->getOgreNode();
        Ogre::SceneNode* snake_cam_node = this->m_SceneMgr->getSceneNode("MainCamNode");
   
  
   
  
        bool isCollision = false;
  
        //RAY TO CHECK COLLISION
        //   if(num_apple_inScene>1){
        //     Ogre::SceneNode* apple_node =
        //   this->m_SceneMgr->getSceneNode("apple_node");
        //     Ogre::Vector3 snake_pos = snake_head_node->getPosition();
        //     Ogre::Ray snake_ray(Ogre::Vector3(snake_pos.x,1,snake_pos.z),direction);
        
        //     Ogre::RaySceneQuery* rayQuery = this->m_SceneMgr->createRayQuery(snake_ray);
        
        //     Ogre::RaySceneQueryResult &result = rayQuery->execute();
        
        //     Ogre::RaySceneQueryResult::iterator itr;
        
        //     for (itr = result.begin(); itr != result.end(); itr++) {
        // 	Ogre::LogManager::getSingletonPtr( )->
        //     logMessage("Name: " + itr->movable->getName());
        //         if (itr->movable->getName().compare("apple")!=0) {
        //             isCollision= true;
        //         }
        //     }
        //   }
        if(isCollision){
                Ogre::LogManager::getSingletonPtr()->logMessage("COLLISION!!");
        }
  
        if(timeDelay>timeDelayMax) {
                timeDelay = 0;
        }
  
        //SNAKE MOVEMENT
        if(timeDelay==0){
                this->mContador++;
                Ogre::Vector3 snake_dir = snake_head_node->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
                Ogre::Vector3 cam_dir = this->m_Camera->getOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
                Ogre::Vector3 snake_position = snake_head_node->getPosition();
                Ogre::Vector3 cam_position = snake_cam_node->getPosition();
       
                //SNAKE UP AND DOWN BEHAVIOR
                if(mRightPress){
                        mSnake->moveRigth();
                        //UNCOMMET FOR FPS CAM OR COMMENT FOR HAVE A PERSPECTIVE VIEW
                        //this->m_Camera->rotate(Ogre::Quaternion(Ogre::Degree(-10), Ogre::Vector3::UNIT_Y));
                }else{
                        if(mLeftPress){
                                mSnake->moveLeft();
                                //UNCOMMET FOR FPS CAM OR COMMENT FOR HAVE A PERSPECTIVE VIEW
                                //this->m_Camera->rotate(Ogre::Quaternion(Ogre::Degree(10), Ogre::Vector3::UNIT_Y));
                        }
                }
                //SNAKE UP AND DOWN BEHAVIOR
                if(mUpPress){
                        mSnake->moveUp();
                        //UNCOMMET FOR FPS CAM OR COMMENT FOR HAVE A PERSPECTIVE VIEW
                        //this->m_Camera->rotate(Ogre::Quaternion(Ogre::Degree(10), Ogre::Vector3::UNIT_Z));
                        //this->m_Camera->rotate(Ogre::Quaternion(Ogre::Degree(10), Ogre::Vector3::UNIT_X));
                }else{
                        if(mDownPress){
                                mSnake->moveDown();
                                //UNCOMMET FOR FPS CAM OR COMMENT FOR HAVE A PERSPECTIVE VIEW
                                //this->m_Camera->rotate(Ogre::Quaternion(Ogre::Degree(-10), Ogre::Vector3::UNIT_Z));
                                //this->m_Camera->rotate(Ogre::Quaternion(Ogre::Degree(-10), Ogre::Vector3::UNIT_X));
                        }
                }
        
                //UNCOMMENT TO HAVE DIFERENT ANGLE OF CAM- COMMENT FOR FPS CAM
                this->m_Camera->lookAt(snake_position);
        
                /*Ogre::LogManager::getSingletonPtr()->logMessage(
                        "snake direction: x:" + to_string(snake_dir.x) + 
                        " y:" + to_string(snake_dir.y) + 
                        " z:" + to_string(snake_dir.z));
                Ogre::LogManager::getSingletonPtr()->logMessage(
                        "cam dir: x:" + to_string(cam_dir.x) + 
                        " y:" + to_string(cam_dir.y) + 
                        " z:" + to_string(cam_dir.z));
                */
                mSnake->draw();
                //if(this->mContador % 50 == 0)
                //        mSnake->addNewVerteb();
        }
        timeDelay = timeDelay + 1;
        
        this->m_Keyboard->capture();
        
        return( true );
}
bool Snake3d::keyPressed( const OIS::KeyEvent& arg )
{
        //#ifdef DIEGO_ENV
        //if(arg.key == 0x6A) { 
        //#else
        if(arg.key == OIS::KC_RIGHT) {
        //#endif
                mRightPress = true;
                mLeftPress = false;
        }
        
        //#ifdef DIEGO_ENV
        //  if(arg.key == 0x69) { 
        //#else 
        if(arg.key == OIS::KC_LEFT) {
        //#endif
                mLeftPress = true;
                mRightPress = false;
        }
        
        if(arg.key == OIS::KC_UP){
                mUpPress = true;
                mDownPress = false;
        }
        
        if(arg.key == OIS::KC_DOWN){
                mUpPress = false;
                mDownPress = true;
        }
}
bool Snake3d::keyReleased( const OIS::KeyEvent& arg )
{
        mRightPress = false;
        mLeftPress = false;
        mUpPress = false;
        mDownPress = false;
}

// eof - $RCSfile$
