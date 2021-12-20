#include "CardTextBehaviorComponent.hpp"

#include <Environment.hpp>
#include <TextComponent.hpp>

#include <algorithm>

#include <iostream>

using DeckOfIllusions::CardTextBehaviorComponent;

/******************************************************************************/
CardTextBehaviorComponent::CardTextBehaviorComponent()
  : Component()
  , mCard(nullptr)
  , mDeck(nullptr)
  , mState(FadeState::eNONE)
  , mFadeTime(0.5)
  , mTimeSpentFading(0.0)
  , mTimeBeganFading(0.0)
{
  /**
   * Connect signals.
   */
  CardRevealed.Connect(*this, [this](CardObject& aCard)
  {
    this->HandleCardRevealed(aCard);
  });

  CardBeganFading.Connect(*this, [this](CardObject& aCard)
  {
    this->HandleCardBeganFading(aCard);
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
void CardTextBehaviorComponent::Update()
{
  switch(mState)
  {
    case FadeState::eFADING_IN:
    {
      mTimeSpentFading = env.GetTime() - mTimeBeganFading;
      float transparency = mTimeSpentFading / mFadeTime;
      transparency = std::min(transparency, 1.0f);

      auto parent = GetParent();
      if(parent != nullptr)
      {
        auto textObj = parent->GetFirstComponentOfType<UrsineEngine::TextComponent>();
        textObj->SetColor(glm::vec4(1.0,
                                    1.0,
                                    1.0,
                                    transparency));
      }

      if(transparency == 1.0f)
      {
        mState = FadeState::eNONE;
      }
      break;
    }
    case FadeState::eFADING_OUT:
    {
      mTimeSpentFading = env.GetTime() - mTimeBeganFading;
      float transparency = 1.0 - (mTimeSpentFading / mFadeTime);
      transparency = std::max(transparency, 0.0f);

      std::cout << transparency << std::endl;

      auto parent = GetParent();
      if(parent != nullptr)
      {
        auto textObj = parent->GetFirstComponentOfType<UrsineEngine::TextComponent>();
        textObj->SetColor(glm::vec4(1.0,
                                    1.0,
                                    1.0,
                                    transparency));
      }

      if(transparency == 0.0f)
      {
        mState = FadeState::eNONE;
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
void CardTextBehaviorComponent::ObserveDeck(DeckObject& aDeck)
{
  mDeck = &aDeck;
  mCard = nullptr;
}

/******************************************************************************/
void CardTextBehaviorComponent::HandleCardRevealed(CardObject& aCard)
{
  if(&aCard == mCard)
  {
    auto parent = GetParent();
    if(parent != nullptr)
    {
      auto textComp = parent->GetFirstComponentOfType<UrsineEngine::TextComponent>();
      if(textComp != nullptr)
      {
        std::string message = "You conjured an image of a(n): ";
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

        // Update the state.
        mTimeBeganFading = env.GetTime();
        mState = FadeState::eFADING_IN;
      }
    }
  }
}

/******************************************************************************/
void CardTextBehaviorComponent::HandleCardBeganFading(CardObject& aCard)
{
  if(&aCard == mCard)
  {
    mTimeBeganFading = env.GetTime();
    mState = FadeState::eFADING_OUT;
  }
}

/******************************************************************************/
void CardTextBehaviorComponent::HandleCardFinishedFading(CardObject& aCard)
{
  if(&aCard == mCard)
  {
    mCard = nullptr;
    mState = FadeState::eNONE;

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
