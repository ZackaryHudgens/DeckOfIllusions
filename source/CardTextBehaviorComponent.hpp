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
       * Updates the component.
       */
      void Update() override;

      /**
       * Sets the deck to observe. When a card is drawn from this deck,
       * the displayed text will change to match the card's description.
       *
       * @param aDeck The DeckObject to observe.
       */
      void ObserveDeck(DeckObject& aDeck);

    private:
      enum class TextState
      {
        eNONE,
        eTUTORIAL,
        eCARD_DATA
      };

      enum class FadeState
      {
        eNONE,
        eFADING_IN,
        eFADING_OUT
      };

      /**
       * A handler function that gets called whenever a deck is shuffled.
       * If this deck is the observed deck, stop displaying the tutorial
       * message.
       *
       * @param aDeck The deck that was shuffled.
       */
      void HandleDeckShuffled(DeckObject& aDeck);

      /**
       * A handler function that gets called whenever a card is revealed
       * to the user. If this card is the drawn card from the given deck,
       * this component will display the card's description.
       *
       * @param aCard The card that finished rotating.
       */
      void HandleCardRevealed(CardObject& aCard);

      /**
       * A handler function that gets called whenever a card begins
       * fading. If this card is the drawn card from the given deck,
       * this component will begin fading the text as well.
       *
       * @param aCard The card that began fading.
       */
      void HandleCardBeganFading(CardObject& aCard);

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

      TextState mTextState;
      FadeState mFadeState;

      float mFadeTime;
      float mTimeSpentFading;
      float mTimeBeganFading;
  };
}

#endif
