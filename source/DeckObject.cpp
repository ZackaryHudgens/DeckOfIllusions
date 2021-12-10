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

/**
void DeckObject::HandleObjectMoved(GameObject* aObject)
{
  CardObject* cardObj = dynamic_cast<CardObject*>(aObject);
  if(cardObj != nullptr)
  {
    switch(mState)
    {
      case DeckState::eIDLE:
      case DeckState::eADDING_CARD:
      {
        break;
      }
      case DeckState::eDRAWING_CARD:
      {
        if(!mCards.empty())
        {
          if(cardObj == mCards.back())
          {
            auto cam = env.GetCurrentScene()->GetDefaultCamera();

            // Stay level with the card.
            cam->SetPosition(glm::vec3(cam->GetPosition().x,
                                       cardObj->GetPosition().y,
                                       cam->GetPosition().z));
          }
        }
        break;
      }
      case DeckState::eWAITING_FOR_FLIP:
      default:
      {
        break;
      }
    }
  }
}

void DeckObject::HandleCardFinishedMoving(CardObject* aCard)
{
  switch(mState)
  {
    case DeckState::eIDLE:
    {
      break;
    }
    case DeckState::eADDING_CARD:
    {
      if(!mCards.empty())
      {
        if(aCard == mCards.back())
        {
          mState = DeckState::eIDLE;
        }
      }
      break;
    }
    case DeckState::eDRAWING_CARD:
    {
      if(!mCards.empty())
      {
        if(aCard == mCards.back())
        {
          mState = DeckState::eWAITING_FOR_FLIP;
        }
      }
      break;
    }
    case DeckState::eWAITING_FOR_FLIP:
    default:
    {
      break;
    }
  }
}

void DeckObject::HandleCardFinishedRotating(CardObject* aCard)
{
  if(!mCards.empty())
  {
    if(aCard == mCards.back())
    {
      switch(mState)
      {
        case DeckState::eWAITING_FOR_FLIP:
        {
          mState = DeckState::eREADING;
          break;
        }
        default:
        {
          break;
        }
      }
    }
  }
}

void DeckObject::HandleCardFinishedFading(CardObject* aCard)
{
  if(!mCards.empty())
  {
    if(aCard == mCards.back())
    {
      switch(mState)
      {
        case DeckState::eREADING:
        {
          auto cam = env.GetCurrentScene()->GetDefaultCamera();
          cam->SetPosition(glm::vec3(0.0, 0.0, 0.0));

          mState = DeckState::eIDLE;
          break;
        }
        default:
        {
          break;
        }
      }
    }
  }
}*/
