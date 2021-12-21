#ifndef DECK_HPP
#define DECK_HPP

#include <vector>

#include "Card.hpp"

namespace DeckOfIllusions
{
  class Deck
  {
    public:

      /**
       * Constructor.
       */
      Deck();

      /**
       * Loads a deck from an external file. See "full_deck.txt" in the
       * resources directory for what a full deck looks like.
       *
       * @param aFile The file to load.
       * @return True if successful, false otherwise.
       */
      bool LoadDeckFromFile(const std::string& aFile);

      /**
       * Writes the deck to an external file.
       *
       * @param aFile The file to write to.
       * @return True if successful, false otherwise.
       */
      bool WriteDeckToFile(const std::string& aFile);

      /**
       * Rearranges the deck at random.
       */
      void Shuffle();

      /**
       * Returns all the cards in the deck.
       *
       * @return A reference to the list of cards in the deck.
       */
      std::vector<Card>& GetCards() { return mCards; }

    private:

      /**
       * Parses a string of data and updates the given card if it's valid.
       * If invalid, this function returns false.
       *
       * @param aCard The card to update.
       * @param aData The data string to parse.
       * @return True if successful, false otherwise.
       */
      bool ParseDataString(Card& aCard,
                           const std::string& aData);

      /**
       * Parses a character and updates the given card with the corresponding
       * suit if it's valid. If invalid, this function returns false.
       *
       * @param aCard The card to update.
       * @param aChar The character to parse.
       * @return True if successful, false otherwise.
       */
      bool ParseSuitCharacter(Card& aCard,
                              const char& aChar);

      /**
       * Parses a character and updates the given card with the corresponding
       * rank if it's valid. If invalid, this function returns false.
       *
       * @param aCard The card to update.
       * @param aChar The character to parse.
       * @return True if successful, false otherwise.
       */
      bool ParseRankCharacter(Card& aCard,
                              const char& aChar);

      /**
       * Returns a character to represent a given suit.
       *
       * @param aSuit The suit to parse.
       * @return A character for the given suit.
       */
      char GetCharacterForSuit(const Suit& aSuit) const;

      /**
       * Returns a character to represent a given rank.
       *
       * @param aRank The rank to parse.
       * @return A character for the given rank.
       */
      char GetCharacterForRank(const Rank& aRank) const;

      std::vector<Card> mCards;
  };
}

#endif
