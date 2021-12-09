#ifndef CARDOBJECT_HPP
#define CARDOBJECT_HPP

#include <GameObject.hpp>

#include "Card.hpp"

namespace DeckOfIllusions
{
  class CardObject : public UrsineEngine::GameObject
  {
    public:

      /**
       * Constructor.
       *
       * @param aName The name for this GameObject.
       */
      CardObject(const std::string& aName = "Card");

      /**
       * Updates this GameObject to reflect the given card.
       *
       * @param aCard The card this object should represent.
       */
      void UpdateCardData(const Card& aCard);

      /**
       * Returns the card this object is based on.
       *
       * @return The card this object is based on.
       */
      Card GetCardData() const { return mCardData; }

    private:
      Card mCardData;
  };

  typedef UrsineEngine::SignalT<CardObject*> CardFinishedMovingSignal;
  typedef UrsineEngine::SignalT<CardObject*> CardFinishedRotatingSignal;
  typedef UrsineEngine::SignalT<CardObject*> CardFinishedFadingSignal;

  extern CardFinishedMovingSignal   CardFinishedMoving;
  extern CardFinishedRotatingSignal CardFinishedRotating;
  extern CardFinishedFadingSignal   CardFinishedFading;
}

#endif
