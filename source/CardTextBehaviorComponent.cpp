#include "CardTextBehaviorComponent.hpp"

#include <TextComponent.hpp>

#include <iostream>

using DeckOfIllusions::CardTextBehaviorComponent;

/******************************************************************************/
CardTextBehaviorComponent::CardTextBehaviorComponent()
  : Component()
  , mCard(nullptr)
  , mDeck(nullptr)
{
  /**
   * Connect signals.
   */
  CardFinishedRotating.Connect(*this, [this](CardObject& aCard)
  {
    this->HandleCardFinishedRotating(aCard);
  });

  CardFinishedFading.Connect(*this, [this](CardObject& aCard)
  {
    this->HandleCardFinishedFading(aCard);
  });

  CardDrawn.Connect(*this, [this](DeckObject& aDeck)
  {
    this->HandleCardDrawn(aDeck);
  });
}

/******************************************************************************/
void CardTextBehaviorComponent::ObserveDeck(DeckObject& aDeck)
{
  mDeck = &aDeck;
  mCard = nullptr;
}

/******************************************************************************/
void CardTextBehaviorComponent::HandleCardFinishedRotating(CardObject& aCard)
{
  if(&aCard == mCard)
  {
    auto parent = GetParent();
    if(parent != nullptr)
    {
      auto textComp = parent->GetFirstComponentOfType<UrsineEngine::TextComponent>();
      if(textComp != nullptr)
      {
        textComp->SetText(mCard->GetCardData().mDescription);
      }
    }
  }
}

/******************************************************************************/
void CardTextBehaviorComponent::HandleCardFinishedFading(CardObject& aCard)
{
  if(&aCard == mCard)
  {
    auto parent = GetParent();
    if(parent != nullptr)
    {
      auto textComp = parent->GetFirstComponentOfType<UrsineEngine::TextComponent>();
      if(textComp != nullptr)
      {
        textComp->SetText("");
      }
    }
  }
}

/******************************************************************************/
void CardTextBehaviorComponent::HandleCardDrawn(DeckObject& aDeck)
{
  if(&aDeck == mDeck)
  {
    auto cards = aDeck.GetChildrenOfType<CardObject>();
    if(!cards.empty())
    {
      mCard = cards.back();
    }
  }
}
