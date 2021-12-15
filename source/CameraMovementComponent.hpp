#ifndef CAMERAMOVEMENTCOMPONENT_HPP
#define CAMERAMOVEMENTCOMPONENT_HPP

#include <Component.hpp>

#include "CardObject.hpp"
#include "DeckObject.hpp"

namespace DeckOfIllusions
{
  class CameraMovementComponent : public UrsineEngine::Component
  {
    public:

      /**
       * Constructor.
       */
      CameraMovementComponent();

      /**
       * Updates the component.
       */
      void Update() override;

      /**
       * Sets the DeckObject to follow. This component will move the camera
       * to follow any cards drawn from the deck.
       *
       * @param aDeck The DeckObject to follow.
       */
      void FollowDeck(DeckObject& aDeck);

    private:

      /**
       * A handler function called whenever a card is drawn from a DeckObject.
       * If the DeckObject is the followed deck, this component will begin
       * following the drawn card.
       *
       * @param aDeck The DeckObject that a card was drawn from.
       */
      void HandleCardDrawn(DeckObject& aDeck);

      /**
       * A handler function called whenever a GameObject moves. If the object
       * is the followed deck or the followed card, the component will move
       * to follow it.
       *
       * @param aObject The GameObject that moved.
       */
      void HandleObjectMoved(UrsineEngine::GameObject* aObject);

      /**
       * A handler function called just before a GameObject is deleted.
       * If the object is the followed deck or the followed card, the
       * appropriate member variable is set to nullptr. Also, if the object
       * was the followed card, this component moves the parent object back
       * to focus on the deck.
       *
       * @param aObject The GameObject that is going to be deleted.
       */
      void HandleObjectPendingDeletion(UrsineEngine::GameObject* aObject);

      CardObject* mFollowedCard;
      DeckObject* mFollowedDeck;

      bool mMovingToDeck;
  };
}

#endif
