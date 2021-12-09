#ifndef CARD_HPP
#define CARD_HPP

#include <string>

namespace DeckOfIllusions
{
  enum class Rank
  {
    eTWO,
    eEIGHT,
    eNINE,
    eTEN,
    eJACK,
    eQUEEN,
    eKING,
    eACE,
    eJOKER
  };

  enum class Suit
  {
    eCLUBS,
    eDIAMONDS,
    eSPADES,
    eHEARTS,
    eJOKER
  };

  struct Card
  {
    Rank mRank;
    Suit mSuit;

    std::string mDescription;
  };
}

#endif
