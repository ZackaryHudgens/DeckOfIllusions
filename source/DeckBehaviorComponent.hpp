#ifndef DECKBEHAVIORCOMPONENT_HPP
#define DECKBEHAVIORCOMPONENT_HPP

#include <CoreSignals.hpp>
#include <Component.hpp>

#include "Card.hpp"
#include "CardObject.hpp"
#include "Deck.hpp"

namespace DeckOfIllusions
{
  class DeckBehaviorComponent : public UrsineEngine::Component
  {
    public:

      /**
       * Constructor.
       */
      DeckBehaviorComponent();

      /**
       * Loads a deck from an external file. See "deck.txt" in the
       * resources directory for what a full deck looks like.
       *
       * @param aFile The file to load.
       * @return True if successful, false otherwise.
       */
      bool LoadDeckFromFile(const std::string& aFile);

      /**
       * Shuffles the deck.
       */
      void Shuffle();

      /**
       * Creates a CardObject from the given card and adds it as a child
       * object of the parent. CardObjects created this way will spawn at
       * a point high above the parent and gradually move downward.
       *
       * @param aCard The card to create an object for.
       */
      void AddCard(const Card& aCard);

    private:
      enum class State
      {
        eIDLE,
        eADDING_CARD,
        eDRAWING_CARD,
        eWAITING_FOR_FLIP,
        eWAITING_FOR_FADE
      };

      /**
       * Adds an index to the given name in order to produce a unique name
       * for a new GameObject.
       *
       * @param aName The name to modify.
       */
      void AddIndexToName(std::string& aName);

      /**
       * A handler function that gets called whenever a key is pressed.
       *
       * @param aCode The key that was pressed.
       * @param aMods Any modifiers present when the key was pressed.
       */
      void HandleKeyPress(const UrsineEngine::KeyCode& aCode,
                          int aMods);

      /**
       * A handler function that gets called whenever a CardObject finishes
       * moving to a destination.
       *
       * @param aCard The CardObject in question.
       */
      void HandleCardFinishedMoving(CardObject& aCard);

      /**
       * A handler function that gets called whenever a CardObject finishes
       * rotating to a specified position.
       *
       * @param aCard The CardObject in question.
       */
      void HandleCardFinishedRotating(CardObject& aCard);

      /**
       * A handler function that gets called whenever a CardObject finishes
       * fading out of view.
       *
       * @param aCard The CardObject in question.
       */
      void HandleCardFinishedFading(CardObject& aCard);

      Deck mDeck;
      State mState;
  };
}

#endif
