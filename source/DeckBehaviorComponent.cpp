#include "DeckBehaviorComponent.hpp"

#include <GameObject.hpp>

#include "DeckObject.hpp"

using DeckOfIllusions::DeckBehaviorComponent;

/******************************************************************************/
DeckBehaviorComponent::DeckBehaviorComponent()
  : Component()
  , mState(State::eIDLE)
{
  UrsineEngine::KeyPressed.Connect(*this, [this](const UrsineEngine::KeyCode& aCode,
                                                 int aMods)
  {
    this->HandleKeyPress(aCode, aMods);
  });
}

/******************************************************************************/
void DeckBehaviorComponent::Update()
{
}

/******************************************************************************/
void DeckBehaviorComponent::HandleKeyPress(const UrsineEngine::KeyCode& aCode,
                                           int aMods)
{
  switch(aCode)
  {
    case UrsineEngine::KeyCode::eKEY_ENTER:
    {
      // Draw a card, flip a card, or fade a card depending on state.
      switch(mState)
      {
        case State::eIDLE:
        {
          break;
        }
        case State::eWAITING_FOR_FLIP:
        {
          break;
        }
        case State::eWAITING_FOR_FADE:
        {
          break;
        }
        default:
        {
          break;
        }
      }
      break;
    }
    case UrsineEngine::KeyCode::eKEY_S:
    {
      // Shuffle the deck if idle.
      switch(mState)
      {
        case State::eIDLE:
        {
          break;
        }
        default:
        {
          break;
        }
      }
      break;
    }
    default:
    {
      break;
    }
  }
}
