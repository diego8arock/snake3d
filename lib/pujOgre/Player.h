namespace pujOgre
{
  /**
   */
  class Player
  {
  public:
    Player( );
  
  private:
    int score = 0;
  public:
    int calculateScore(int score);
    void destroy();
  };

}
