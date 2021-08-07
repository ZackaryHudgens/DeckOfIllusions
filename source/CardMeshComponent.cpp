#include "CardMeshComponent.hpp"

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(ShaderLib_Deck);

using UrsineRenderer::MeshVertex;
using UrsineRenderer::Shader;

using DeckOfIllusions::CardMeshComponent;

CardMeshComponent::CardMeshComponent(const CardData& aData)
  : MeshComponent()
{
  SetupShaders();
  SetupVertexInfo(aData);
  SetCurrentShader("cardVanishingShader");
}

void CardMeshComponent::SetupShaders()
{
  auto fs = cmrc::ShaderLib_Deck::get_filesystem();
  auto vertexFile = fs.open("resources/shaders/CardShader.vert");
  auto fragmentFile = fs.open("resources/shaders/CardShader.frag");

  Shader cardShader(vertexFile.begin(), fragmentFile.begin());
  cardShader.SetInt("texSampler", 0);
  AddShader("cardShader", cardShader);

  fs = cmrc::ShaderLib_Deck::get_filesystem();
  vertexFile = fs.open("resources/shaders/CardVanishingShader.vert");
  fragmentFile = fs.open("resources/shaders/CardVanishingShader.frag");

  Shader cardVanishingShader(vertexFile.begin(), fragmentFile.begin());
  cardShader.SetInt("texSampler", 0);
  AddShader("cardVanishingShader", cardVanishingShader);
}

void CardMeshComponent::SetupVertexInfo(const CardData& aData)
{
  // Create the 3D vertices for thix card.
  MeshVertex vertex;

  // Add the deck texture to this card.
  Texture deck;
  deck.CreateTextureFromFile("resources/textures/deck.png");
  AddTexture(deck);

  // Determine the NDC for a single card.
  double magnitude = sqrt((pow(539, 2) + pow(766, 2)));
  double xVal = 539.0 / magnitude;
  double yVal = 766.0 / magnitude;

  // Front side
  vertex.mPosition = glm::vec3(-xVal, -yVal, 0.0);
  vertex.mColor = glm::vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aData, Corner::eBOTTOM_LEFT);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(xVal, -yVal, 0.0);
  vertex.mColor = glm::vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aData, Corner::eBOTTOM_RIGHT);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(xVal, yVal, 0.0);
  vertex.mColor = glm::vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords = GetTextureCoords(deck, aData, Corner::eTOP_RIGHT);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(-xVal, yVal, 0.0);
  vertex.mColor = glm::vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aData, Corner::eTOP_LEFT);
  AddVertex(vertex);

  // Back side
  vertex.mPosition = glm::vec3(-xVal, -yVal, -0.01);
  vertex.mColor = glm::vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aData, Corner::eBOTTOM_LEFT, true);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(xVal, -yVal, -0.01);
  vertex.mColor = glm::vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords = GetTextureCoords(deck, aData, Corner::eBOTTOM_RIGHT, true);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(xVal, yVal, -0.01);
  vertex.mColor = glm::vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aData, Corner::eTOP_RIGHT, true);
  AddVertex(vertex);
  vertex.mPosition = glm::vec3(-xVal, yVal, -0.01);
  vertex.mColor = glm::vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords = GetTextureCoords(deck, aData, Corner::eTOP_LEFT, true);
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

glm::vec2 CardMeshComponent::GetTextureCoords(const Texture& aTexture,
                                              const CardData& aData,
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
    switch(aData.mSuit)
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
    switch(aData.mRank)
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
