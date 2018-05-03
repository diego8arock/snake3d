#include <iostream>
#include <chrono>
#include <ctime>
#include <pujOgre/Level.h>
#include <OgreVector3.h>

pujOgre::Level::Level()
{
  this->limitTime = std::chrono::system_clock::now();
  this->limitInSeconds = 60*1;
  this->maxOfApples = 1;
  this->spanSnakePosition = Ogre::Vector3(0,0,0);
}

pujOgre::Level::Level(double minDuration, int limitApples, Ogre::Vector3 initialSnakePos)
{
  this->limitTime = std::chrono::system_clock::now();
  if(minDuration>0.0){
    this->limitInSeconds = 60 * minDuration;
  }else{
    this->limitInSeconds = 60 * 1;
  }
  if(limitApples>0){
    this->maxOfApples = limitApples;
  }else{
    this->maxOfApples = 1;
  }
  this->spanSnakePosition = initialSnakePos;
}

void pujOgre::Level::addApple()
{

}

bool pujOgre::Level::isEndOfLevel()
{
  std::chrono::system_clock::time_point timeMark = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = timeMark-this->limitTime;
  if(elapsed_seconds.count()<limitInSeconds){
    return false;
  }else{
    return true;
  }
}

void pujOgre::Level::destroy()
{

}



