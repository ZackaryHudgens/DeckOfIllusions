#include "CardObject.hpp"

#include <UrsineEngine/Environment.hpp>

#include "CardMeshComponent.hpp"
#include "CardMovementComponent.hpp"

using DeckOfIllusions::CardObject;

CardObject::CardObject(const CardData& aData, const std::string& aName)
  : GameObject(aName)
  , mData(aData)
{
  // Handle invalid data due to Jokers
  if(mData.mRank == Rank::eJOKER && mData.mSuit != Suit::eJOKER)
  {
    mData.mSuit = Suit::eJOKER;
  }
  if(mData.mSuit == Suit::eJOKER && mData.mRank != Rank::eJOKER)
  {
    mData.mRank = Rank::eJOKER;
  }

  auto cardMesh = std::make_unique<CardMeshComponent>(mData);
  AddComponent(std::move(cardMesh));

  auto cardMovement = std::make_unique<CardMovementComponent>();
  AddComponent(std::move(cardMovement));
}

DeckOfIllusions::CardFinishedMovingSignal DeckOfIllusions::CardFinishedMoving;
DeckOfIllusions::CardFinishedRotatingSignal DeckOfIllusions::CardFinishedRotating;
