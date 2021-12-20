#include "DeckBehaviorComponent.hpp"

#include <GameObject.hpp>

#include <iostream>

#include "CardMeshComponent.hpp"
#include "CardMovementComponent.hpp"
#include "CardObject.hpp"
#include "DeckObject.hpp"

using DeckOfIllusions::DeckBehaviorComponent;

/******************************************************************************/
DeckBehaviorComponent::DeckBehaviorComponent()
  : Component()
  , mState(State::eIDLE)
{
  /**
   * Connect signals.
   */
  UrsineEngine::KeyPressed.Connect(*this, [this](const UrsineEngine::KeyCode& aCode,
                                                 int aMods)
  {
    this->HandleKeyPress(aCode, aMods);
  });

  CardFinishedMoving.Connect(*this, [this](CardObject& aCard)
  {
    this->HandleCardFinishedMoving(aCard);
  });

  CardFinishedRotating.Connect(*this, [this](CardObject& aCard)
  {
    this->HandleCardFinishedRotating(aCard);
  });

  CardFinishedFading.Connect(*this, [this](CardObject& aCard)
  {
    this->HandleCardFinishedFading(aCard);
  });
}

/******************************************************************************/
bool DeckBehaviorComponent::LoadDeckFromFile(const std::string& aFile)
{
  bool success = false;
  success = mDeck.LoadDeckFromFile(aFile);

  if(success)
  {
    auto parent = GetParent();
    if(parent != nullptr)
    {
      // Remove all CardObjects from the parent GameObject.
      for(const auto& cardObj : parent->GetChildrenOfType<CardObject>())
      {
        parent->RemoveChild(cardObj->GetName());
      }

      // For each card in the deck, create a new CardObject and add it.
      for(const auto& card : mDeck.GetCards())
      {
        AddCard(card);
      }
    }
  }

  return success;
}

/******************************************************************************/
void DeckBehaviorComponent::Shuffle()
{
  mDeck.Shuffle();

  auto parent = GetParent();
  if(parent != nullptr)
  {
    // Update each child CardObject to reflect the new card order.
    int index = 0;
    for(auto& cardObj : parent->GetChildrenOfType<CardObject>())
    {
      if(index <= mDeck.GetCards().size())
      {
        cardObj->UpdateCardData(mDeck.GetCards()[index]);
        ++index;
      }
      else
      {
        break;
      }
    }
  }
}

/******************************************************************************/
void DeckBehaviorComponent::AddCard(const Card& aCard)
{
  auto parent = GetParent();
  if(parent != nullptr)
  {
    // A card can only be added while the deck is idle or while
    // another card is being added.
    switch(mState)
    {
      case State::eIDLE:
      case State::eADDING_CARD:
      {
        // Update the state.
        mState = State::eADDING_CARD;

        //mDeck.GetCards().emplace_back(aCard);

        // Create a new CardObject.
        std::string name = "Card";
        AddIndexToName(name);
        auto cardObj = std::make_unique<CardObject>(name);
        cardObj->UpdateCardData(aCard);

        // Rotate the CardObject to face away from the viewer.
        cardObj->Rotate(180, glm::vec3(0.0, 1.0, 0.0));

        // Rotate the CardObject to lie flat.
        cardObj->Rotate(55, glm::vec3(1.0, 0.0, 0.0));

        // Position the CardObject high above the top of the deck.
        std::vector<CardObject*> cards = parent->GetChildrenOfType<CardObject>();
        glm::vec3 topOfDeck = glm::vec3(parent->GetPosition().x,
                                        (0.01 * cards.size()),
                                        parent->GetPosition().z);
        cardObj->Translate(glm::vec3(topOfDeck.x,
                                     topOfDeck.y + (2.0 * (cards.size() + 1)),
                                     topOfDeck.z));

        // Begin moving the CardObject to the top of the deck.
        for(auto& moveComponent : cardObj->GetComponentsOfType<CardMovementComponent>())
        {
          moveComponent->MoveTo(topOfDeck, 0.3);
        }

        // Finally, add the CardObject as a child of the parent GameObject.
        parent->AddChild(std::move(cardObj));
        break;
      }
      default:
      {
        break;
      }
    }
  }
}

/******************************************************************************/
void DeckBehaviorComponent::AddIndexToName(std::string& aName)
{
  auto parent = GetParent();
  if(parent != nullptr)
  {
    std::string newName = aName;
    int index = 2;
    while(parent->GetChild(newName) != nullptr)
    {
      newName = aName;
      newName.append("_" + std::to_string(index));
      ++index;
    }

    aName = newName;
  }
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
          // Draw a card by moving it upwards and rotating it so its back is
          // facing the viewer.
          auto parent = GetParent();
          if(parent != nullptr)
          {
            auto cardObjects = parent->GetChildrenOfType<CardObject>();
            if(!cardObjects.empty())
            {
              auto moveComponent = cardObjects.back()->GetFirstComponentOfType<CardMovementComponent>();
              if(moveComponent != nullptr)
              {
                glm::vec3 destination = glm::vec3(parent->GetPosition().x,
                                                  2.0,
                                                  parent->GetPosition().z);
                moveComponent->MoveTo(destination,
                                      0.3);
                moveComponent->RotateTo(-55,
                                        glm::vec3(1.0, 0.0, 0.0),
                                        0.3);

                mState = State::eDRAWING_CARD;

                auto deckObj = dynamic_cast<DeckObject*>(parent);
                if(deckObj != nullptr)
                {
                  CardDrawn.Notify(*deckObj);
                }
              }
            }
          }
          break;
        }
        case State::eWAITING_FOR_FLIP:
        {
          // Rotate the drawn card to face the viewer.
          auto parent = GetParent();
          if(parent != nullptr)
          {
            auto cardObjects = parent->GetChildrenOfType<CardObject>();
            if(!cardObjects.empty())
            {
              auto moveComponent = cardObjects.back()->GetFirstComponentOfType<CardMovementComponent>();
              if(moveComponent != nullptr)
              {
                moveComponent->RotateTo(180,
                                        glm::vec3(0.0, 1.0, 0.0),
                                        0.3);
              }
            }
          }
          break;
        }
        case State::eWAITING_FOR_FADE:
        {
          // Tell the card's mesh to begin fading out of view.
          auto parent = GetParent();
          if(parent != nullptr)
          {
            auto cardObjects = parent->GetChildrenOfType<CardObject>();
            if(!cardObjects.empty())
            {
              auto meshComponent = cardObjects.back()->GetFirstComponentOfType<CardMeshComponent>();
              if(meshComponent != nullptr)
              {
                meshComponent->BeginFading();
              }
            }
          }
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
          Shuffle();
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

/******************************************************************************/
void DeckBehaviorComponent::HandleCardFinishedMoving(CardObject& aCard)
{
  switch(mState)
  {
    case State::eADDING_CARD:
    {
      // If the card that finished moving was the most recently added card,
      // then the deck is now idle.
      auto parent = GetParent();
      if(parent != nullptr)
      {
        if(!parent->GetChildrenOfType<CardObject>().empty())
        {
          auto cardObj = parent->GetChildrenOfType<CardObject>().back();
          if(&aCard == cardObj)
          {
            mState = State::eIDLE;
          }
        }
      }
      break;
    }
    case State::eDRAWING_CARD:
    {
      // If the card that finished moving was the card that was being drawn,
      // then the deck is now waiting for the user to flip it.
      auto parent = GetParent();
      if(parent != nullptr)
      {
        if(!parent->GetChildrenOfType<CardObject>().empty())
        {
          auto cardObj = parent->GetChildrenOfType<CardObject>().back();
          if(&aCard == cardObj)
          {
            mState = State::eWAITING_FOR_FLIP;
          }
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

/******************************************************************************/
void DeckBehaviorComponent::HandleCardFinishedRotating(CardObject& aCard)
{
  switch(mState)
  {
    case State::eWAITING_FOR_FLIP:
    {
      // If the card that finished rotating was the drawn card,
      // then the deck is now waiting for it to fade away.
      auto parent = GetParent();
      if(parent != nullptr)
      {
        if(!parent->GetChildrenOfType<CardObject>().empty())
        {
          auto cardObj = parent->GetChildrenOfType<CardObject>().back();
          if(&aCard == cardObj)
          {
            mState = State::eWAITING_FOR_FADE;
            CardRevealed.Notify(*cardObj);
          }
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

/******************************************************************************/
void DeckBehaviorComponent::HandleCardFinishedFading(CardObject& aCard)
{
  switch(mState)
  {
    case State::eWAITING_FOR_FADE:
    {
      // If the card that finished fading was the drawn card,
      // then it gets removed and the deck goes back to being idle.
      auto parent = GetParent();
      if(parent != nullptr)
      {
        if(!parent->GetChildrenOfType<CardObject>().empty())
        {
          auto cardObj = parent->GetChildrenOfType<CardObject>().back();
          if(&aCard == cardObj)
          {
            // Remove the card from the deck and from the parent object.
            mDeck.GetCards().pop_back();
            parent->RemoveChild(cardObj->GetName());

            mState = State::eIDLE;
          }
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
