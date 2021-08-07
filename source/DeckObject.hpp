#ifndef DECKOBJECT_HPP
#define DECKOBJECT_HPP

#include <UrsineEngine/GameObject.hpp>

#include "CardObject.hpp"

using UrsineCore::GameObject;

namespace DeckOfIllusions
{
  enum class DeckState
  {
    eIDLE,
    eADDING_CARD,
    eDRAWING_CARD,
    eWAITING_FOR_FLIP
  };

  class DeckObject : public GameObject
  {
    public:
      DeckObject(const std::string& aName = "Deck");

      bool LoadDeckFromFile(const std::string& aFile);
      void AddCard(const CardData& aData);

      DeckState GetState() const { return mState; }

      void Draw();
      void FlipCard();
      void Shuffle();

    private:
      bool ParseDataString(CardData& aData,
                           const std::string& aLine);
      bool GetSuitFromCharacter(CardData& aData,
                                const char& aChar);
      bool GetRankFromCharacter(CardData& aData,
                                const char& aChar);
      void AddIndexToName(std::string& aName) const;

      void HandleObjectMoved(GameObject* aObject);
      void HandleCardFinishedMoving(CardObject* aCard);
      void HandleCardFinishedRotating(CardObject* aCard);

      DeckState mState;

      std::vector<CardObject*> mCards;
  };
}

#endif
