#include "CardTextBehaviorComponent.hpp"

#include <Environment.hpp>
#include <TextComponent.hpp>

#include <algorithm>
#include <sstream>

#include <iostream>

using DeckOfIllusions::CardTextBehaviorComponent;

/******************************************************************************/
CardTextBehaviorComponent::CardTextBehaviorComponent()
  : Component()
  , mCard(nullptr)
  , mDeck(nullptr)
  , mTextState(TextState::eNONE)
  , mFadeState(FadeState::eNONE)
  , mFadeTime(0.5)
  , mTimeSpentFading(0.0)
  , mTimeBeganFading(0.0)
{
  /**
   * Connect signals.
   */
  DeckShuffled.Connect(*this, [this](DeckObject& aDeck)
  {
    this->HandleDeckShuffled(aDeck);
  });

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
void CardTextBehaviorComponent::Update(double aTime)
{
  switch(mFadeState)
  {
    case FadeState::eFADING_IN:
    {
      mTimeSpentFading = aTime - mTimeBeganFading;
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
        mFadeState = FadeState::eNONE;
      }
      break;
    }
    case FadeState::eFADING_OUT:
    {
      mTimeSpentFading = aTime - mTimeBeganFading;
      float transparency = 1.0 - (mTimeSpentFading / mFadeTime);
      transparency = std::max(transparency, 0.0f);

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
        mFadeState = FadeState::eNONE;
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

  auto parent = GetParent();
  if(parent != nullptr)
  {
    auto textComp = parent->GetFirstComponentOfType<UrsineEngine::TextComponent>();
    if(textComp != nullptr)
    {
      std::stringstream message;
      message << "Press [S] to shuffle the deck.";
      message << "\n";
      message << "Press [Enter] to draw a card.";

      textComp->SetText(message.str());
      textComp->SetColor(glm::vec4(1.0,
                                   1.0,
                                   1.0,
                                   1.0));

      // Update the text state.
      mTextState = TextState::eTUTORIAL;

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

/******************************************************************************/
void CardTextBehaviorComponent::HandleDeckShuffled(DeckObject& aDeck)
{
  switch(mTextState)
  {
    case TextState::eTUTORIAL:
    {
      // Don't display the tutorial anymore.
      mTextState = TextState::eCARD_DATA;
      mFadeState = FadeState::eFADING_OUT;

      mTimeBeganFading = env.GetTime();
      break;
    }
    default:
    {
      break;
    }
  }
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
        // Create the notification message.
        std::stringstream message;
        message << "You conjured an illusion of ";

        if(mCard->GetCardData().mDescription != "Yourself")
        {
          char firstChar = mCard->GetCardData().mDescription.front();
          if((firstChar == 'a' || firstChar == 'A') ||
             (firstChar == 'e' || firstChar == 'E') ||
             (firstChar == 'i' || firstChar == 'I') ||
             (firstChar == 'o' || firstChar == 'O') ||
             (firstChar == 'u' || firstChar == 'U'))
          {
            message << " an ";
          }
          else
          {
            message << " a ";
          }
        }
        message << mCard->GetCardData().mDescription;
        message << "!";

        textComp->SetText(message.str());

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
        mFadeState = FadeState::eFADING_IN;
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
    mFadeState = FadeState::eFADING_OUT;
  }
}

/******************************************************************************/
void CardTextBehaviorComponent::HandleCardFinishedFading(CardObject& aCard)
{
  if(&aCard == mCard)
  {
    mCard = nullptr;
    mFadeState = FadeState::eNONE;

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
    switch(mTextState)
    {
      case TextState::eTUTORIAL:
      {
        // Don't display the tutorial anymore.
        mTextState = TextState::eCARD_DATA;
        mFadeState = FadeState::eFADING_OUT;

        mTimeBeganFading = env.GetTime();
        break;
      }
      default:
      {
        break;
      }
    }

    auto cards = aDeck.GetChildrenOfType<CardObject>();
    if(!cards.empty())
    {
      mCard = cards.back();
    }
  }
}
