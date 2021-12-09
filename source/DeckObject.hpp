#ifndef DECKOBJECT_HPP
#define DECKOBJECT_HPP

#include <GameObject.hpp>

#include "CardObject.hpp"
#include "Deck.hpp"

namespace DeckOfIllusions
{
  class DeckObject : public UrsineEngine::GameObject
  {
    public:

      /**
       * Constructor.
       *
       * @param aName The name for this GameObject.
       */
      DeckObject(const std::string& aName = "Deck");

      /**
       * Loads a deck from an external file. See "deck.txt" in the
       * resources directory for what a full deck looks like.
       *
       * @param aFile The file to load.
       * @return True if successful, false otherwise.
       */
      bool LoadDeckFromFile(const std::string& aFile);

      void Draw();
      void FlipCard();
      void FadeCard();
      void Shuffle();

    private:
      void AddIndexToName(std::string& aName) const;

      void HandleObjectMoved(GameObject* aObject);
      void HandleCardFinishedMoving(CardObject* aCard);
      void HandleCardFinishedRotating(CardObject* aCard);
      void HandleCardFinishedFading(CardObject* aCard);

      Deck mDeck;
  };
}

#endif
