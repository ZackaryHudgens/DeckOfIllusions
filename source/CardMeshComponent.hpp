#ifndef CARDMESHCOMPONENT_HPP
#define CARDMESHCOMPONENT_HPP

#include <UrsineEngine/MeshComponent.hpp>
#include <UrsineEngine/Texture.hpp>

#include "CardObject.hpp"

using UrsineRenderer::MeshComponent;
using UrsineRenderer::Texture;

namespace DeckOfIllusions
{
  class CardMeshComponent : public MeshComponent
  {
    public:
      CardMeshComponent(const CardData& aData);

    private:
      enum class Corner
      {
        eTOP_LEFT,
        eTOP_RIGHT,
        eBOTTOM_LEFT,
        eBOTTOM_RIGHT
      };

      void SetupShaders();
      void SetupVertexInfo(const CardData& aData);

      glm::vec2 GetTextureCoords(const Texture& aTexture,
                                 const CardData& aData,
                                 const Corner& aCorner,
                                 bool aBack = false);
  };
}

#endif
