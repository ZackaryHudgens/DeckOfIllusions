#include "DeckInputComponent.hpp"

#include <UrsineEngine/GameObject.hpp>

#include "DeckObject.hpp"

using DeckOfIllusions::DeckInputComponent;

DeckInputComponent::DeckInputComponent()
  : Component()
{
  UrsineCore::KeyPressed.Connect(*this, [this](const UrsineCore::KeyCode& aCode,
                                               int aMods)
  {
    this->HandleKeyPress(aCode, aMods);
  });
}

void DeckInputComponent::HandleKeyPress(const UrsineCore::KeyCode& aCode,
                                        int aMods)
{
  switch(aCode)
  {
    case UrsineCore::KeyCode::eKEY_ENTER:
    {
      DeckObject* deckObj = dynamic_cast<DeckObject*>(GetParent());
      if(deckObj != nullptr)
      {
        switch(deckObj->GetState())
        {
          case DeckState::eIDLE:
          {
            deckObj->Draw();
            break;
          }
          case DeckState::eWAITING_FOR_FLIP:
          {
            deckObj->FlipCard();
            break;
          }
          case DeckState::eADDING_CARD:
          case DeckState::eDRAWING_CARD:
          default:
          {
            break;
          }
        }
      }
      break;
    }
    case UrsineCore::KeyCode::eKEY_S:
    {
      DeckObject* deckObj = dynamic_cast<DeckObject*>(GetParent());
      if(deckObj != nullptr)
      {
        deckObj->Shuffle();
      }
      break;
    }
    default:
    {
      break;
    }
  }
}
