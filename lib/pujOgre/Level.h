#include <chrono>
#include <ctime>
#include <OgreVector3.h>

namespace Ogre
{
  class OverlaySystem;
}
namespace pujOgre{
  
  class Level{
public:
  Level();
  Level(double minDuration,int limitApples, Ogre::Vector3 initialSnakePos);
private:
  std::chrono::system_clock::time_point limitTime;
  double limitInSeconds;
  int maxOfApples;
  Ogre::Vector3 spanSnakePosition;
public:
  void addApple();
  bool isEndOfLevel();
  void destroy();
};

}