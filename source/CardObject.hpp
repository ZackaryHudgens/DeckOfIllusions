#ifndef CARDOBJECT_HPP
#define CARDOBJECT_HPP

#include <UrsineEngine/GameObject.hpp>

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

  struct CardData
  {
    Rank mRank;
    Suit mSuit;
    std::string mDescription;
  };

  class CardObject : public UrsineCore::GameObject
  {
    public:
      CardObject(const CardData& aData, const std::string& aName = "Card");

      CardData GetData() const { return mData; }

    private:
      CardData mData;
  };

  typedef UrsineCore::SignalT<CardObject*> CardFinishedMovingSignal;
  typedef UrsineCore::SignalT<CardObject*> CardFinishedRotatingSignal;

  extern CardFinishedMovingSignal   CardFinishedMoving;
  extern CardFinishedRotatingSignal CardFinishedRotating;
}

#endif
