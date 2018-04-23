#include <iostream>
#include <pujOgre/Application.h>

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

/**
 */
class Snake3d
  : public pujOgre::Application
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
  Ogre::AnimationState* m_AnimationState;
  int timeDelay =0;
  int timeDelayMax = 150;
  float snake_velocity = 1;
  int game_difficulty = 0;
  bool right_press = false;
  bool left_press = false;
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
Snake3d::
Snake3d( )
  : pujOgre::Application( )
{
  if(game_difficulty==0){
    snake_velocity = 1;
    timeDelayMax = 10;
  }else{
    if(game_difficulty==1){
     snake_velocity = 2;
     timeDelayMax = 9;
    }
  }
}

// -------------------------------------------------------------------------
Snake3d::
~Snake3d( )
{
}

// -------------------------------------------------------------------------
void Snake3d::
createCamera( )
{
  this->pujOgre::Application::createCamera( );
  this->m_Camera->setPosition( Ogre::Vector3( 25, 25, 25 ) );
  this->m_Camera->lookAt( Ogre::Vector3( 0, 10, 0 ) );
  this->m_Camera->setNearClipDistance( 5 );
}

// -------------------------------------------------------------------------
void Snake3d::
createScene( )
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
  Ogre::MeshManager::getSingleton( ).
    createPlane(
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
  Ogre::Entity* snake_head =
    this->m_SceneMgr->createEntity(
      "snake_head", "snake_head.mesh"
      );
  snake_head->setCastShadows( true );
  Ogre::AxisAlignedBox bbox = snake_head->getBoundingBox( );

  // Associate it to a node
  Ogre::SceneNode* snake_head_node =
    this->m_SceneMgr->getRootSceneNode( )->createChildSceneNode(
      "snake_head_node"
      );
    
  Ogre::SceneNode* snake_cam_node =
  this->m_SceneMgr->getSceneNode("MainCamNode");
 
  snake_cam_node->translate( 0, -bbox.getMinimum( )[ 1 ], 0 );
  snake_cam_node->setPosition(3,0,3);
  this->m_Camera->lookAt(Ogre::Vector3( 0, 0, 0 ));
  
  snake_head_node->attachObject( snake_head );
  snake_head_node->translate( 0, -bbox.getMinimum( )[ 1 ], 0 );

 /* // Prepare skeleton to be manually controlled
  Ogre::SkeletonInstance* snake_head_skel = snake_head->getSkeleton( );
  for( unsigned int bIt = 0; bIt < snake_head_skel->getNumBones( ); bIt++ )
    snake_head_skel->getBone( bIt )->setManuallyControlled( true );*/
}

// -------------------------------------------------------------------------
bool Snake3d::
frameRenderingQueued( const Ogre::FrameEvent& evt )
{
  Ogre::SceneNode* snake_head_node =
  this->m_SceneMgr->getSceneNode("snake_head_node");
  Ogre::SceneNode* snake_cam_node =
  this->m_SceneMgr->getSceneNode("MainCamNode");
  
  if(timeDelay>timeDelayMax){
    timeDelay = 0;
  }
  
  if(timeDelay==0){
    if(right_press){
      snake_head_node->setPosition( snake_head_node->getPosition().x, snake_head_node->getPosition().y, snake_head_node->getPosition().z-snake_velocity);    
      snake_head_node->yaw(Ogre::Radian(-0.1));
      
      snake_cam_node->setPosition( snake_cam_node->getPosition().x, snake_cam_node->getPosition().y, snake_cam_node->getPosition().z-snake_velocity);    
      snake_cam_node->yaw(Ogre::Radian(-0.1));
      
    }else{
      if(left_press){
	snake_head_node->setPosition( snake_head_node->getPosition().x-snake_velocity, snake_head_node->getPosition().y, snake_head_node->getPosition().z);  
	snake_head_node->yaw(Ogre::Radian(0.1));
	
	snake_cam_node->setPosition( snake_cam_node->getPosition().x-snake_velocity, snake_cam_node->getPosition().y, snake_cam_node->getPosition().z);    
	snake_cam_node->yaw(Ogre::Radian(0.1));
      }else{
	snake_head_node->setPosition( snake_head_node->getPosition().x-snake_velocity, snake_head_node->getPosition().y, snake_head_node->getPosition().z-snake_velocity);    
	snake_cam_node->setPosition( snake_cam_node->getPosition().x-snake_velocity, snake_cam_node->getPosition().y, snake_cam_node->getPosition().z-snake_velocity);    
	
      }
    }
      
  Ogre::LogManager::getSingletonPtr( )->
    logMessage( "x: " + Ogre::StringConverter::toString(snake_head_node->getPosition().x) + " y: " + Ogre::StringConverter::toString(snake_head_node->getPosition().y) + " z: " + Ogre::StringConverter::toString(snake_head_node->getPosition().z));
  }
  //snake_cam_node->yaw(Ogre::Radian(0.1));
  //snake_cam_node->roll(Ogre::Radian(0.1));
	//snake_cam_node->pitch(Ogre::Radian(0.1));
  timeDelay = timeDelay + 1;
//   if( this->pujOgre::Application::frameRenderingQueued( evt ) )
//   {
//     return( true );
//   }
//   else
//     return( false );
  //CAPTURES
  this->m_Keyboard->capture();
  
  return( true );
}
bool Snake3d::
keyPressed( const OIS::KeyEvent& arg )
{
  Ogre::LogManager::getSingletonPtr( )->
  logMessage("pressed");
  if(arg.key == OIS::KC_RIGHT){
  Ogre::LogManager::getSingletonPtr( )->
    logMessage("Right pressed");
    right_press = true;
  }
  if(arg.key == OIS::KC_LEFT){
  Ogre::LogManager::getSingletonPtr( )->
    logMessage("Left pressed");
    left_press = true;
  }
}
bool Snake3d::
keyReleased( const OIS::KeyEvent& arg )
{
    right_press = false;
    left_press = false;
}

// eof - $RCSfile$
