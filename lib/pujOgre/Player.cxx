#include <pujOgre/Player.h>

/**
 * Player
 */

pujOgre::Player::Player()
{
  this->score=0;
}
int pujOgre::Player::calculateScore(int score)
{
  this->score = this->score + score;
}

void pujOgre::Player::destroy()
{

}



