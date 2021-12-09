#include "DeckObject.hpp"

#include "DeckBehaviorComponent.hpp"

using DeckOfIllusions::DeckObject;

/******************************************************************************/
DeckObject::DeckObject(const std::string& aName)
  : GameObject(aName)
{
  /**
   * Connect signals.
   */
  UrsineEngine::ObjectMoved.Connect(*this, [this](GameObject* aObject)
  {
    this->HandleObjectMoved(aObject);
  });

  CardFinishedMoving.Connect(*this, [this](CardObject* aCard)
  {
    this->HandleCardFinishedMoving(aCard);
  });

  CardFinishedRotating.Connect(*this, [this](CardObject* aCard)
  {
    this->HandleCardFinishedRotating(aCard);
  });

  CardFinishedFading.Connect(*this, [this](CardObject* aCard)
  {
    this->HandleCardFinishedFading(aCard);
  });

  /**
   * Add components.
   */
  AddComponent(std::make_unique<DeckBehaviorComponent>());
}

/******************************************************************************/
bool DeckObject::LoadDeckFromFile(const std::string& aFile)
{
  bool success = false;
  success = mDeck.LoadDeckFromFile(aFile);

  if(success)
  {
    // Add a card for each one in the deck.
  }

  return success;
}

/**
 * Adds a card to the DeckObject, initially placing it high above the
 * top of the deck, then moving it downward.
 *
 * @param aData The CardData to assign to this card.
 */
void DeckObject::AddCard(const CardData& aData)
{
  // Update our state.
  mState = DeckState::eADDING_CARD;

  // Create a new card object with this data.
  std::string name = "Card";
  AddIndexToName(name);
  auto cardObj = std::make_unique<CardObject>(aData, name);

  // Rotate the card to face away from the viewer.
  cardObj->Rotate(180, glm::vec3(0.0, 1.0, 0.0));

  // Rotate the card to lie flat.
  cardObj->Rotate(55, glm::vec3(1.0, 0.0, 0.0));

  // Position the card high above the top of the deck.
  std::vector<CardObject*> cards = GetChildrenOfType<CardObject>();
  glm::vec3 topOfDeck = glm::vec3(GetPosition().x,
                                  (0.01 * cards.size()),
                                  GetPosition().z);
  cardObj->Translate(glm::vec3(topOfDeck.x,
                               topOfDeck.y + (2.0 * cards.size()),
                               topOfDeck.z));

  // Move the card to the top of the deck.
  for(auto& moveComponent : cardObj->GetComponentsOfType<CardMovementComponent>())
  {
    moveComponent->MoveTo(topOfDeck, 0.3);
  }

  // Add the card as a child GameObject.
  AddChild(std::move(cardObj));

  // Keep track of the order of cards.
  CardObject* card = dynamic_cast<CardObject*>(GetChild(name));
  if(card != nullptr)
  {
    mCards.emplace_back(card);
  }
}

/**
 * Draws a card, moving it upward and waiting for the user to 
 * flip it.
 */
void DeckObject::Draw()
{
  switch(mState)
  {
    case DeckState::eIDLE:
    {
      if(!mCards.empty())
      {
        mState = DeckState::eDRAWING_CARD;

        // Move the card high above the deck,
        // and rotate it back to vertical.
        for(auto& moveComponent : mCards.back()->GetComponentsOfType<CardMovementComponent>())
        {
          glm::vec3 dest = glm::vec3(GetPosition().x,
                                     2.0,
                                     GetPosition().z);
          moveComponent->MoveTo(dest, 0.3);
          moveComponent->RotateTo(-55, glm::vec3(1.0, 0.0, 0.0), 0.3);
        }
      }
      break;
    }
    case DeckState::eADDING_CARD:
    case DeckState::eDRAWING_CARD:
    case DeckState::eWAITING_FOR_FLIP:
    default:
    {
      break;
    }
  }
}

/**
 * Flips the drawn card if we're in the correct state.
 */
void DeckObject::FlipCard()
{
  switch(mState)
  {
    case DeckState::eIDLE:
    case DeckState::eADDING_CARD:
    case DeckState::eDRAWING_CARD:
    {
      break;
    }
    case DeckState::eWAITING_FOR_FLIP:
    {
      if(!mCards.empty())
      {
        // Rotate the card around to face the viewer.
        for(auto& moveComponent : mCards.back()->GetComponentsOfType<CardMovementComponent>())
        {
          moveComponent->RotateTo(180, glm::vec3(0.0, 1.0, 0.0), 0.3);
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
void DeckObject::FadeCard()
{
  switch(mState)
  {
    case DeckState::eREADING:
    {
      if(!mCards.empty())
      {
        auto mesh = mCards.back()->GetFirstComponentOfType<CardMeshComponent>();
        if(mesh != nullptr)
        {
          mesh->BeginFading();
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

void DeckObject::Shuffle()
{
  switch(mState)
  {
    case DeckState::eIDLE:
    {
      // Clear the list of card references.
      mCards.clear();

      std::vector<CardData> dataList;
      for(auto& obj : GetChildren())
      {
        auto cardObj = dynamic_cast<CardObject*>(obj);
        if(cardObj != nullptr)
        {
          // Grab this card's data and place it in the temporary list.
          dataList.emplace_back(cardObj->GetData());

          // Remove this CardObject.
          RemoveChild(obj->GetName());
        }
      }

      // Shuffle the card data.
      std::random_shuffle(dataList.begin(), dataList.end());

      // Re-add the cards in their new random order.
      for(const auto& data : dataList)
      {
        AddCard(data);
      }
      break;
    }
    case DeckState::eADDING_CARD:
    case DeckState::eDRAWING_CARD:
    case DeckState::eWAITING_FOR_FLIP:
    default:
    {
      break;
    }
  }
}

bool DeckObject::ParseDataString(CardData& aData,
                                 const std::string& aLine)
{
  bool success = true;
  bool readingDescription = false;

  if(aLine.size() < 2)
  {
    success = false;
  }
  else
  {
    for(int i = 0; i < aLine.size(); ++i)
    {
      if(!success)
      {
        break;
      }

      if(i == 0)
      {
        success = GetSuitFromCharacter(aData, aLine[i]);
      }
      else if (i == 1)
      {
        success = GetRankFromCharacter(aData, aLine[i]);
      }
      else if(readingDescription)
      {
        if(aLine[i] == '\"')
        {
          readingDescription = false;
          break;
        }
        else
        {
          aData.mDescription += aLine[i];
        }
      }
      else if(aLine[i] == '\"')
      {
        readingDescription = true;
      }
    }
  }

  return success;
}

bool DeckObject::GetSuitFromCharacter(CardData& aData,
                                      const char& aChar)
{
  bool success = true;

  if(aChar == 'c')
  {
    aData.mSuit = Suit::eCLUBS;
  }
  else if(aChar == 'd')
  {
    aData.mSuit = Suit::eDIAMONDS;
  }
  else if(aChar == 's')
  {
    aData.mSuit = Suit::eSPADES;
  }
  else if(aChar == 'h')
  {
    aData.mSuit = Suit::eHEARTS;
  }
  else if(aChar == 'j')
  {
    aData.mSuit = Suit::eJOKER;
  }
  else
  {
    success = false;
  }

  return success;
}

bool DeckObject::GetRankFromCharacter(CardData& aData,
                                      const char& aChar)
{
  bool success = true;

  if(aData.mSuit == Suit::eJOKER)
  {
    aData.mRank = Rank::eJOKER;
  }
  else if(aChar == '2')
  {
    aData.mRank = Rank::eTWO;
  }
  else if(aChar == '8')
  {
    aData.mRank = Rank::eEIGHT;
  }
  else if(aChar == '9')
  {
    aData.mRank = Rank::eNINE;
  }
  else if(aChar == 't')
  {
    aData.mRank = Rank::eTEN;
  }
  else if(aChar == 'j')
  {
    aData.mRank = Rank::eJACK;
  }
  else if(aChar == 'k')
  {
    aData.mRank = Rank::eKING;
  }
  else if(aChar == 'q')
  {
    aData.mRank = Rank::eQUEEN;
  }
  else if(aChar == 'a')
  {
    aData.mRank = Rank::eACE;
  }
  else
  {
    success = false;
  }

  return success;
}

/**
 * A function that adds an index to a string
 * in order to create a unique name for a GameObject.
 *
 * @param aName The name to modify.
 */
void DeckObject::AddIndexToName(std::string& aName) const
{
  std::string newName = aName;
  int index = 0;
  while(GetChild(newName) != nullptr)
  {
    newName = aName;
    newName.append("_" + std::to_string(index));
    ++index;
  }

  aName = newName;
}

/**
 * A handler function for the ObjectMoved signal.
 * If the GameObject in question is the card that's being
 * drawn, then we move the scene's default camera to follow it.
 */
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

/**
 * A handler function for the CardFinishedMoving signal.
 * If the CardObject in question is the most recently added
 * card, then the deck can transition to an idle state.
 *
 * @param aCard The CardObject that finished moving.
 */
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

/******************************************************************************/
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

/******************************************************************************/
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
}
