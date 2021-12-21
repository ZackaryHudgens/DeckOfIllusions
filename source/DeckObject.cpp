#include "DeckObject.hpp"

#include "DeckBehaviorComponent.hpp"

using DeckOfIllusions::DeckObject;

/******************************************************************************/
DeckObject::DeckObject(const std::string& aName)
  : GameObject(aName)
{
  /**
   * Add components.
   */
  AddComponent(std::make_unique<DeckBehaviorComponent>());
}

/******************************************************************************/
bool DeckObject::LoadDeckFromFile(const std::string& aFile)
{
  bool success = false;

  auto behaviorComp = GetFirstComponentOfType<DeckBehaviorComponent>();
  if(behaviorComp != nullptr)
  {
    success = behaviorComp->LoadDeckFromFile(aFile);
  }

  return success;
}

DeckOfIllusions::DeckShuffledSignal DeckOfIllusions::DeckShuffled;
DeckOfIllusions::CardDrawnSignal    DeckOfIllusions::CardDrawn;
