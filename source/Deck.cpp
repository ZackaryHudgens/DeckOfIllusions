#include "Deck.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using DeckOfIllusions::Deck;

/******************************************************************************/
Deck::Deck()
{
}

/******************************************************************************/
bool Deck::LoadDeckFromFile(const std::string& aFile)
{
  bool success = true;

  std::ifstream in;
  in.open(aFile);

  success = in.is_open();
  if(success)
  {
    std::string line;
    while(std::getline(in, line))
    {
      Card card;
      success = ParseDataString(card, line);
      if(success)
      {
        mCards.emplace_back(card);
      }
      else
      {
        std::cout << "Couldn't parse line: " << line << std::endl;
      }
    }
  }
  else
  {
    std::cout << "Couldn't open file: " << aFile << std::endl;
  }

  in.close();

  return success;
}

/******************************************************************************/
bool Deck::WriteDeckToFile(const std::string& aFile)
{
  bool success = true;

  std::ofstream out;
  out.open(aFile, std::ios_base::out | std::ios_base::trunc);

  success = out.is_open();
  if(success)
  {
    std::stringstream ss;
    for(const auto& card : mCards)
    {
      ss << GetCharacterForSuit(card.mSuit);
      ss << GetCharacterForRank(card.mRank);
      ss << " \"" << card.mDescription << "\"";
      ss << "\n";

      out << ss.str();
      ss.str("");
    }
  }

  out.close();

  return success;
}

/******************************************************************************/
void Deck::Shuffle()
{
  std::random_shuffle(mCards.begin(), mCards.end());
}

/******************************************************************************/
bool Deck::ParseDataString(Card& aCard,
                           const std::string& aData)
{
  bool success = true;
  bool readingDescription = false;

  if(aData.size() < 2)
  {
    success = false;
  }
  else
  {
    for(int i = 0; i < aData.size(); ++i)
    {
      if(!success)
      {
        break;
      }

      if(i == 0)
      {
        success = ParseSuitCharacter(aCard, aData[i]);
      }
      else if (i == 1)
      {
        success = ParseRankCharacter(aCard, aData[i]);
      }
      else if(readingDescription)
      {
        if(aData[i] == '\"')
        {
          readingDescription = false;
          break;
        }
        else
        {
          aCard.mDescription += aData[i];
        }
      }
      else if(aData[i] == '\"')
      {
        readingDescription = true;
      }
    }
  }

  return success;
}

/******************************************************************************/
bool Deck::ParseSuitCharacter(Card& aCard,
                              const char& aChar)
{
  bool success = true;

  if(aChar == 'c')
  {
    aCard.mSuit = Suit::eCLUBS;
  }
  else if(aChar == 'd')
  {
    aCard.mSuit = Suit::eDIAMONDS;
  }
  else if(aChar == 's')
  {
    aCard.mSuit = Suit::eSPADES;
  }
  else if(aChar == 'h')
  {
    aCard.mSuit = Suit::eHEARTS;
  }
  else if(aChar == 'j')
  {
    aCard.mSuit = Suit::eJOKER;
  }
  else
  {
    success = false;
  }

  return success;
}

/******************************************************************************/
bool Deck::ParseRankCharacter(Card& aCard,
                              const char& aChar)
{
  bool success = true;

  if(aCard.mSuit == Suit::eJOKER)
  {
    aCard.mRank = Rank::eJOKER;
  }
  else if(aChar == '2')
  {
    aCard.mRank = Rank::eTWO;
  }
  else if(aChar == '8')
  {
    aCard.mRank = Rank::eEIGHT;
  }
  else if(aChar == '9')
  {
    aCard.mRank = Rank::eNINE;
  }
  else if(aChar == 't')
  {
    aCard.mRank = Rank::eTEN;
  }
  else if(aChar == 'j')
  {
    aCard.mRank = Rank::eJACK;
  }
  else if(aChar == 'k')
  {
    aCard.mRank = Rank::eKING;
  }
  else if(aChar == 'q')
  {
    aCard.mRank = Rank::eQUEEN;
  }
  else if(aChar == 'a')
  {
    aCard.mRank = Rank::eACE;
  }
  else
  {
    success = false;
  }

  return success;
}

/******************************************************************************/
char Deck::GetCharacterForSuit(const Suit& aSuit) const
{
  char value;

  switch(aSuit)
  {
    case Suit::eCLUBS:
    {
      value = 'c';
      break;
    }
    case Suit::eDIAMONDS:
    {
      value = 'd';
      break;
    }
    case Suit::eHEARTS:
    {
      value = 'h';
      break;
    }
    case Suit::eSPADES:
    {
      value = 's';
      break;
    }
    case Suit::eJOKER:
    {
      value = 'j';
      break;
    }
  }

  return value;
}

/******************************************************************************/
char Deck::GetCharacterForRank(const Rank& aRank) const
{
  char value;

  switch(aRank)
  {
    case Rank::eACE:
    {
      value = 'a';
      break;
    }
    case Rank::eTWO:
    {
      value = '2';
      break;
    }
    case Rank::eEIGHT:
    {
      value = '8';
      break;
    }
    case Rank::eNINE:
    {
      value = '9';
      break;
    }
    case Rank::eTEN:
    {
      value = 't';
      break;
    }
    case Rank::eJACK:
    {
      value = 'j';
      break;
    }
    case Rank::eQUEEN:
    {
      value = 'q';
      break;
    }
    case Rank::eKING:
    {
      value = 'k';
      break;
    }
    case Rank::eJOKER:
    {
      value = 'j';
      break;
    }
  }

  return value;
}
