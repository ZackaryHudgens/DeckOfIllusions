#ifndef CARDMESHCOMPONENT_HPP
#define CARDMESHCOMPONENT_HPP

#include <MeshComponent.hpp>
#include <Texture.hpp>

#include "CardObject.hpp"

namespace DeckOfIllusions
{
  class CardMeshComponent : public UrsineEngine::MeshComponent
  {
    public:

      /**
       * Constructor.
       */
      CardMeshComponent();

      /**
       * Updates the component.
       *
       * @param aTime The start time of the current Scene's Update().
       */
      void Update(double aTime) override;

      /**
       * Updates the mesh's texture to reflect the given card.
       *
       * @param aCard The card this mesh should display.
       */
      void UpdateCardData(const Card& aCard);

      /**
       * Begins fading the card out of view. Once the card is fully
       * transparent, the CardFinishedFading signal is notified in Update().
       */
      void BeginFading();

    private:
      enum class Corner
      {
        eTOP_LEFT,
        eTOP_RIGHT,
        eBOTTOM_LEFT,
        eBOTTOM_RIGHT
      };

      /**
       * Creates the shader object(s) for this component.
       */
      void SetupShaders();

      /**
       * Determines the model and texture coordinates for this mesh based on
       * the given card.
       *
       * @param aCard The card this mesh should display.
       */
      void SetupVertexInfo(const Card& aCard);

      /**
       * Returns the texture coordinates for a given corner of a given card.
       *
       * @param aTexture A texture atlas containing all images of all cards.
       * @param aCard The card this mesh should display.
       * @param aCorner The corner of the card to find on the atlas.
       * @param aBack Whether to calculate coordinates for the front or back
       *              of the card.
       * @return A 2D vector containing the texture coordinates for the given
       *         corner of the given card.
       */
      glm::vec2 GetTextureCoords(const UrsineEngine::Texture& aTexture,
                                 const Card& aCard,
                                 const Corner& aCorner,
                                 bool aBack = false);

      float mFadeTime;
      float mTimeSpentFading;
      float mTimeBeganFading;

      bool mFading;
  };
}

#endif
