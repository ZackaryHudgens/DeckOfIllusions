#include "CardMeshComponent.hpp"

#include <iostream>

#include <Environment.hpp>

using DeckOfIllusions::CardMeshComponent;

int gCardWidthInPixels = 539;
int gCardHeightInPixels = 766;

/******************************************************************************/
CardMeshComponent::CardMeshComponent()
  : MeshComponent()
  , mFadeTime(0.5)
  , mTimeSpentFading(0.0)
  , mTimeBeganFading(0.0)
  , mFading(false)
{
  SetupShaders();
  SetCurrentShader("cardShader");
}

/******************************************************************************/
void CardMeshComponent::Update()
{
  if(mFading)
  {
    mTimeSpentFading = env.GetTime() - mTimeBeganFading;
    float transparency = 1.0 - (mTimeSpentFading / mFadeTime);

    GetCurrentShader()->Activate();
    GetCurrentShader()->SetFloat("transparency", transparency);

    if(transparency <= 0.0)
    {
      auto cardObj = dynamic_cast<CardObject*>(GetParent());
      CardFinishedFading.Notify(cardObj);
    }
  }
}

/******************************************************************************/
void CardMeshComponent::UpdateCardData(const Card& aCard)
{
  SetupVertexInfo(aCard);
}

/******************************************************************************/
void CardMeshComponent::BeginFading()
{
  mTimeBeganFading = env.GetTime();
  mFading = true;
}

/******************************************************************************/
void CardMeshComponent::SetupShaders()
{
  std::string vertexFile = "resources/shaders/CardShader.vert";
  std::string fragmentFile = "resources/shaders/CardShader.frag";

  UrsineEngine::Shader cardShader(vertexFile, fragmentFile);
  cardShader.Activate();
  cardShader.SetFloat("transparency", 1.0);
  AddShader("cardShader", cardShader);
}

/******************************************************************************/
void CardMeshComponent::SetupVertexInfo(const Card& aCard)
{
  // Remove any previous vertices and textures
  // Create the 3D vertices for thix card.
  UrsineEngine::MeshVertex vertex;

  // Add the deck texture to this card.
  UrsineEngine::Texture deck;
  deck.CreateTextureFromFile("resources/textures/deck.png");
  AddTexture(deck);

  // Create model space coordinates for the card by normalizing
  // its width and height in pixels.
  double magnitude = sqrt((pow(gCardWidthInPixels, 2) + pow(gCardHeightInPixels, 2)));
  double width = (double)gCardWidthInPixels / magnitude;
  double height = (double)gCardHeightInPixels / magnitude;

  double xVal = width / 2.0;
  double yVal = height / 2.0;

  // Front side
  vertex.mPosition = glm::vec3(-xVal, -yVal, 0.0);
  vertex.mColor = glm::vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aCard, Corner::eBOTTOM_LEFT);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(xVal, -yVal, 0.0);
  vertex.mColor = glm::vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aCard, Corner::eBOTTOM_RIGHT);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(xVal, yVal, 0.0);
  vertex.mColor = glm::vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords = GetTextureCoords(deck, aCard, Corner::eTOP_RIGHT);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(-xVal, yVal, 0.0);
  vertex.mColor = glm::vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aCard, Corner::eTOP_LEFT);
  AddVertex(vertex);

  // Back side
  vertex.mPosition = glm::vec3(-xVal, -yVal, -0.01);
  vertex.mColor = glm::vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aCard, Corner::eBOTTOM_LEFT, true);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(xVal, -yVal, -0.01);
  vertex.mColor = glm::vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords = GetTextureCoords(deck, aCard, Corner::eBOTTOM_RIGHT, true);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(xVal, yVal, -0.01);
  vertex.mColor = glm::vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aCard, Corner::eTOP_RIGHT, true);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(-xVal, yVal, -0.01);
  vertex.mColor = glm::vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aCard, Corner::eTOP_LEFT, true);
  AddVertex(vertex);

  // Specify the order in which to draw these vertices.
  // Front side
  AddIndex(0);
  AddIndex(1);
  AddIndex(2);
  AddIndex(2);
  AddIndex(3);
  AddIndex(0);

  // Right side
  AddIndex(1);
  AddIndex(5);
  AddIndex(6);
  AddIndex(6);
  AddIndex(2);
  AddIndex(1);

  // Back side
  AddIndex(7);
  AddIndex(6);
  AddIndex(5);
  AddIndex(5);
  AddIndex(4);
  AddIndex(7);

  // Left side
  AddIndex(4);
  AddIndex(0);
  AddIndex(3);
  AddIndex(3);
  AddIndex(7);
  AddIndex(4);

  // Bottom
  AddIndex(4);
  AddIndex(5);
  AddIndex(1);
  AddIndex(1);
  AddIndex(0);
  AddIndex(4);

  // Top
  AddIndex(3);
  AddIndex(2);
  AddIndex(6);
  AddIndex(6);
  AddIndex(7);
  AddIndex(3);
}

glm::vec2 CardMeshComponent::GetTextureCoords(const UrsineEngine::Texture& aTexture,
                                              const Card& aCard,
                                              const Corner& aCorner,
                                              bool aBack)
{
  int row, column = 0;

  if(aBack)
  {
    row = 5;
    column = 2;
  }
  else
  {
    // Suits are organized in the deck texture by row.
    switch(aCard.mSuit)
    {
      case Suit::eCLUBS:
      {
        row = 1;
        break;
      }
      case Suit::eDIAMONDS:
      {
        row = 2;
        break;
      }
      case Suit::eSPADES:
      {
        row = 3;
        break;
      }
      case Suit::eHEARTS:
      {
        row = 4;
        break;
      }
      case Suit::eJOKER:
      {
        row = 5;
        break;
      }
    }

    // Each row of a suit is placed in ascending order by rank.
    switch(aCard.mRank)
    {
      case Rank::eTWO:
      case Rank::eJOKER:
      {
        column = 1;
        break;
      }
      case Rank::eEIGHT:
      {
        column = 2;
        break;
      }
      case Rank::eNINE:
      {
        column = 3;
        break;
      }
      case Rank::eTEN:
      {
        column = 4;
        break;
      }
      case Rank::eJACK:
      {
        column = 5;
        break;
      }
      case Rank::eQUEEN:
      {
        column = 6;
        break;
      }
      case Rank::eKING:
      {
        column = 7;
        break;
      }
      case Rank::eACE:
      {
        column = 8;
        break;
      }
    }
  }

  // Use the row and column to determine the card's texture coordinates.
  double width = aTexture.GetData().mWidth;
  double height = aTexture.GetData().mHeight;
  double cardWidth = 539.0;
  double cardHeight = 766.0;
  double xCoord, yCoord = 0.0;

  switch(aCorner)
  {
    case Corner::eTOP_LEFT:
    {
      xCoord = (cardWidth * (column - 1)) / width;
      yCoord = (cardHeight * (6 - row)) / height;
      break;
    }
    case Corner::eTOP_RIGHT:
    {
      xCoord = (cardWidth * column) / width;
      yCoord = (cardHeight * (6 - row)) / height;
      break;
    }
    case Corner::eBOTTOM_LEFT:
    {
      xCoord = (cardWidth * (column - 1)) / width;
      yCoord = (cardHeight * (5 - row)) / height;
      break;
    }
    case Corner::eBOTTOM_RIGHT:
    {
      xCoord = (cardWidth * column) / width;
      yCoord = (cardHeight * (5 - row)) / height;
      break;
    }
    default:
    {
      break;
    }
  }

  return glm::vec2(xCoord, yCoord);
}
