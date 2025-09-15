
/*

http://en.wikipedia.org/wiki/Amen_break

C | ---- ---- ---- ----                                                  | ---- ---- ---- ----                                                     | ---- ---- ---- ----                                                     |---- ---- --X- ----|
R | x-x- x-x- x-x- x-x-                                                  | x-x- x-x- x-x- x-x-                                                     | x-x- x-x- x-X- x-x-                                                     |x-x- x-x- x--- x-x-|
S | ---- o--o -o-- o--o                                                  | ---- o--o -o-- o--o                                                     | ---- o--o -o-- --o-                                                     |-o-- o--o -o-- --o-|
B | o-o- ---- --oo ----                                                  | o-o- ---- --oo ----                                                     | o-o- ---- --o- ----                                                     |--oo ---- --o- ----|
  | 1 +  2 +  3 +  4 +                                                   | 1 +  2 +  3 +  4 +                                                      | 1 +  2 +  3 +  4 +                                                      |1 +  2 +  3 +  4 + |
  
*/


unsigned char amenBreak[4][64] = 
{
/* empty channel - add you own melody or add 001 to play a user selected pitch on channel 0 - upto you. */
  {  0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0,   0,  0,  0,  0, /*|*/  0, 0, 0, 0,  0, 0, 0, 0, 000,000,000,000, 000,000,000,000, /*|*/ 000,000,000,000, 000,000,000,000, 000,000,000,000, 000,000,000,000, /*|*/ 000,000,000,000, 000,000,000,000, 000,000,000,000, 000,000,000,000 },

/*Ride*/
  {001,000,001,000, 001,000,001,000, 001,000,001,000, 001,000,001,000, /*|*/ 001,000,001,000, 001,000,001,000, 001,000,001,000, 001,000,001,000, /*|*/ 001,000,001,000, 001,000,001,000, 001,000,002,000, 001,000,001,000, /*|*/ 001,000,001,000, 001,000,001,000, 001,000,003,000, 001,000,001,000 },
/*Snare*/
  {000,000,000,000, 200,000,000,100, 000,100,000,000, 500,000,000,100, /*|*/ 000,000,000,000, 200,000,000,100, 000,100,000,000, 050,000,000,100, /*|*/ 000,000,000,000, 200,000,000,100, 000,100,000,000, 000,000,050,000, /*|*/ 000,100,000,000, 200,000,000,100, 000,100,000,000, 000,000,050,000 },
/*Bass*/  
  {100,000,100,000, 000,000,000,000, 000,000,100,100, 000,000,000,000, /*|*/ 001,000,001,000, 000,000,000,000, 000,000,001,001, 000,000,000,000, /*|*/ 001,000,001,000, 000,000,000,000, 000,000,001,000, 000,000,000,000, /*|*/ 000,000,001,001, 000,000,000,000, 000,000,001,000, 000,000,000,000 },
};

/* Make your own 64 beat loops - use different voices to add drum, bass, of melodies */

unsigned char yourTrack[4][64] = 
{
/* empty channel - add you own melody or add 001 to play a user selected pitch on channel 0 - upto you. */
  {100,000,100,000, 000,000,000,000, 000,000,100,100, 000,000,000,000, /*|*/ 001,000,001,000, 000,000,000,000, 000,000,001,001, 000,000,000,000, /*|*/ 001,000,001,000, 000,000,000,000, 000,000,001,000, 000,000,000,000, /*|*/ 000,000,001,001, 000,000,000,000, 000,000,001,000, 000,000,000,000 },

/*Ride*/
  {001,000,001,000, 001,000,001,000, 001,000,001,000, 001,000,001,000, /*|*/ 001,000,001,000, 001,000,001,000, 001,000,001,000, 001,000,001,000, /*|*/ 001,000,001,000, 001,000,001,000, 001,000,002,000, 001,000,001,000, /*|*/ 001,000,001,000, 001,000,001,000, 001,000,003,000, 001,000,001,000 },
/*Snare*/
  {000,000,000,000, 200,000,000,100, 000,100,000,000, 500,000,000,100, /*|*/ 000,000,000,000, 200,000,000,100, 000,100,000,000, 050,000,000,100, /*|*/ 000,000,000,000, 200,000,000,100, 000,100,000,000, 000,000,050,000, /*|*/ 000,100,000,000, 200,000,000,100, 000,100,000,000, 000,000,050,000 },
/*Bass*/  
  {100,000,100,000, 000,000,000,000, 000,000,100,100, 000,000,000,000, /*|*/ 001,000,001,000, 000,000,000,000, 000,000,001,001, 000,000,000,000, /*|*/ 001,000,001,000, 000,000,000,000, 000,000,001,000, 000,000,000,000, /*|*/ 000,000,001,001, 000,000,000,000, 000,000,001,000, 000,000,000,000 },
};

unsigned char originalTrack[CHANNEL_MAX][32]=
{
	{1,0,0,0,   1,0,0,0,  1,0,0,0,   1,0,0,0,  1,0,0,0,   1,0,0,0,  1,0,0,0,   1,0,0,0},              // played by Channel 0
	{48,0,0,41, 0,0,44,0, 44,0,0,46, 0,0,41,0, 48,0,0,41, 0,0,44,0, 0,44,0,46, 0,0,51,0},  // played by Channel 1
	{0,0,0,50,  0,0,0,0,  0,0,0,50,  0,0,0,0,  0,0,0,50,  0,0,0,0,  0,0,0,50,  0,0,0,0},          // played by Channel 2
	{0,0,1,0,   0,0,1,0,  0,0,1,0,   0,0,1,0,  0,0,1,0,   0,0,1,0,  0,0,1,0,   0,0,1,0}               // Played by channel 3
};


class CSequence
{
  public:
    CSequence(unsigned char *pSequence0,unsigned char *pSequence1,unsigned char *pSequence2,unsigned char *pSequence3,unsigned char sLength)
    {
      m_pSequenceTrack0 = pSequence0;
      m_pSequenceTrack1 = pSequence1;
      m_pSequenceTrack2 = pSequence2;
      m_pSequenceTrack3 = pSequence3;
      m_sLength = sLength;
    }
    unsigned char getTrigger(unsigned char sChannel,unsigned char sBeat)
    {
      unsigned char sTrigger = 0;
      if(sBeat < m_sLength)
      {
        switch(sChannel)
        {
          case 0:
            if(m_pSequenceTrack0 != 0)
             sTrigger = m_pSequenceTrack0[sBeat];
             break;
          case 1:
            if(m_pSequenceTrack1 != 0)
             sTrigger = m_pSequenceTrack1[sBeat];
             break;
          case 2:
            if(m_pSequenceTrack2 != 0)
             sTrigger = m_pSequenceTrack2[sBeat];
             break;
          case 3:
            if(m_pSequenceTrack3 != 0)
             sTrigger = m_pSequenceTrack3[sBeat];
             break;
        }
      }
      return sTrigger;
    }
    uint8_t getLength()
    {
      return m_sLength;
    }
  protected:
    unsigned char *m_pSequenceTrack0;
    unsigned char *m_pSequenceTrack1;
    unsigned char *m_pSequenceTrack2;
    unsigned char *m_pSequenceTrack3;

    unsigned char m_sLength;
};

// 11570

//CSequence amenSequence(amenBreak[0],amenBreak[1],amenBreak[2],amenBreak[3],64);
//CSequence yourSequence(yourTrack[0],yourTrack[1],yourTrack[2],yourTrack[3],64);
CSequence originalSequence(originalTrack[0],originalTrack[1],originalTrack[2],originalTrack[3],32);

CSequence *pCurrentSequence = &originalSequence;
