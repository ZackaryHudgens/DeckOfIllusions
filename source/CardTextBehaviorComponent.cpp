#include "CardTextBehaviorComponent.hpp"

#include <Environment.hpp>
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
        std::string message = "an image of a(n): ";
        message += mCard->GetCardData().mDescription;
        message += "!";
        textComp->SetText(message);

        // Update the parent object's position to remain centered horizontally.
        auto dimensions = env.GetWindowDimensions();
        auto textWidth = textComp->GetWidth();
        double advance = (double)dimensions.x - (double)textWidth;
        advance /= 2.0;

        auto position = parent->GetPosition();
        parent->SetPosition(glm::vec3(advance,
                                      position.y,
                                      position.z));
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
