#ifndef CARDTEXTBEHAVIORCOMPONENT_HPP
#define CARDTEXTBEHAVIORCOMPONENT_HPP

#include <Component.hpp>

#include "CardObject.hpp"
#include "DeckObject.hpp"

namespace DeckOfIllusions
{
  class CardTextBehaviorComponent : public UrsineEngine::Component
  {
    public:

      /**
       * Constructor.
       */
      CardTextBehaviorComponent();

      /**
       * Sets the deck to observe. When a card is drawn from this deck,
       * the displayed text will change to match the card's description.
       *
       * @param aDeck The DeckObject to observe.
       */
      void ObserveDeck(DeckObject& aDeck);

    private:

      /**
       * A handler function that gets called whenever a card finishes
       * rotating to a specified degree. If this card is the drawn card
       * from the given deck, this component will update the text
       * of the parent object to reflect the card.
       *
       * @param aCard The card that finished rotating.
       */
      void HandleCardFinishedRotating(CardObject& aCard);

      /**
       * A handler function that gets called whenever a card finishes
       * fading. If this card is the drawn card from the given deck,
       * this component will set the text of the parent object to an
       * empty string.
       *
       * @param aCard The card that finished fading.
       */
      void HandleCardFinishedFading(CardObject& aCard);

      /**
       * A handler function that gets called whenever a deck draws a card.
       * If the deck is the given deck, this component keeps track of the
       * drawn card.
       *
       * @param aDeck The deck that drew a card.
       */
      void HandleCardDrawn(DeckObject& aDeck);

      CardObject* mCard;
      DeckObject* mDeck;
  };
}

#endif
