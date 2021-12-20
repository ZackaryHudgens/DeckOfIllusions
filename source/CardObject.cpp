#include "CardObject.hpp"

#include "CardMeshComponent.hpp"
#include "CardMovementComponent.hpp"

using DeckOfIllusions::CardObject;

/******************************************************************************/
CardObject::CardObject(const std::string& aName)
  : GameObject(aName)
{
  auto cardMesh = std::make_unique<CardMeshComponent>();
  AddComponent(std::move(cardMesh));

  auto cardMovement = std::make_unique<CardMovementComponent>();
  AddComponent(std::move(cardMovement));
}

/******************************************************************************/
void CardObject::UpdateCardData(const Card& aCard)
{
  mCardData = aCard;

  // Update the mesh to display the new card.
  auto mesh = GetFirstComponentOfType<CardMeshComponent>();
  if(mesh != nullptr)
  {
    mesh->UpdateCardData(mCardData);
  }
}

DeckOfIllusions::CardFinishedMovingSignal   DeckOfIllusions::CardFinishedMoving;
DeckOfIllusions::CardFinishedRotatingSignal DeckOfIllusions::CardFinishedRotating;
DeckOfIllusions::CardBeganFadingSignal      DeckOfIllusions::CardBeganFading;
DeckOfIllusions::CardFinishedFadingSignal   DeckOfIllusions::CardFinishedFading;
DeckOfIllusions::CardRevealedSignal         DeckOfIllusions::CardRevealed;
