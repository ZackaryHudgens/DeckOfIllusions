#include "CameraMovementComponent.hpp"

using DeckOfIllusions::CameraMovementComponent;

/******************************************************************************/
CameraMovementComponent::CameraMovementComponent()
  : Component()
  , mFollowedDeck(nullptr)
  , mFollowedCard(nullptr)
  , mMovingToDeck(false)
{
  /**
   * Connect signals.
   */
  CardDrawn.Connect(*this, [this](DeckObject& aDeck)
  {
    this->HandleCardDrawn(aDeck);
  });

  UrsineEngine::ObjectMoved.Connect(*this, [this](UrsineEngine::GameObject* aObject)
  {
    this->HandleObjectMoved(aObject);
  });

  UrsineEngine::ObjectPendingDeletion.Connect(*this, [this](UrsineEngine::GameObject* aObject)
  {
    this->HandleObjectPendingDeletion(aObject);
  });
}

/******************************************************************************/
void CameraMovementComponent::Update()
{
  if(mMovingToDeck)
  {
    auto parent = GetParent();
    if(parent != nullptr && mFollowedDeck != nullptr)
    {
      auto position = parent->GetPosition();
      auto destination = mFollowedDeck->GetPosition();
      destination.z = position.z;

      auto vec = glm::mix(position,
                          destination,
                          0.3);

      if(fabs(vec.x - destination.x) < 0.005 &&
         fabs(vec.y - destination.y) < 0.005)
      {
        // Finalize the translation.
        parent->SetPosition(glm::vec3(destination.x,
                                      destination.y,
                                      destination.z));
        mMovingToDeck = false;
      }
      else
      {
        parent->SetPosition(vec);
      }
    }
  }
}

/******************************************************************************/
void CameraMovementComponent::FollowDeck(DeckOfIllusions::DeckObject& aDeck)
{
  auto parent = GetParent();
  if(parent != nullptr)
  {
    parent->SetPosition(glm::vec3(aDeck.GetPosition().x,
                                  aDeck.GetPosition().y,
                                  aDeck.GetPosition().z + 3.0));
    mFollowedDeck = &aDeck;
  }
}

/******************************************************************************/
void CameraMovementComponent::HandleCardDrawn(DeckOfIllusions::DeckObject& aDeck)
{
  if(&aDeck == mFollowedDeck)
  {
    auto cards = mFollowedDeck->GetChildrenOfType<CardObject>();
    if(!cards.empty())
    {
      mFollowedCard = cards.back();

      if(mMovingToDeck)
      {
        mMovingToDeck = false;
      }
    }
  }
}

/******************************************************************************/
void CameraMovementComponent::HandleObjectMoved(UrsineEngine::GameObject* aObject)
{
  if(aObject == mFollowedCard)
  {
    auto parent = GetParent();
    if(parent != nullptr)
    {
      parent->SetPosition(glm::vec3(mFollowedCard->GetPosition().x,
                                    mFollowedCard->GetPosition().y,
                                    mFollowedCard->GetPosition().z + 3.0));

      if(mMovingToDeck)
      {
        mMovingToDeck = false;
      }
    }
  }
}

/******************************************************************************/
void CameraMovementComponent::HandleObjectPendingDeletion(UrsineEngine::GameObject* aObject)
{
  // If the object to be deleted is the followed deck, just set the member
  // variables to nullptr. If the object to be deleted is the followed
  // card, then begin moving back down towards the deck.
  if(aObject == mFollowedDeck)
  {
    mFollowedDeck = nullptr;
    mFollowedCard = nullptr;
  }
  else if(aObject == mFollowedCard)
  {
    mFollowedCard = nullptr;
    mMovingToDeck = true;
  }
}
