#ifndef DECKOBJECT_HPP
#define DECKOBJECT_HPP

#include <GameObject.hpp>

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
       * This function will fail if this object does not have a
       * DeckBehaviorComponent attached.
       *
       * @param aFile The file to load.
       * @return True if successful, false otherwise.
       */
      bool LoadDeckFromFile(const std::string& aFile);

      /**
       * Shuffles the deck.
       *
       * This function will do nothing if this object does not have a
       * DeckBehaviorComponent attached.
       */
      void Shuffle();
  };

  typedef UrsineEngine::SignalT<DeckObject&>              CardDrawnSignal;

  extern CardDrawnSignal    CardDrawn;
}

#endif
