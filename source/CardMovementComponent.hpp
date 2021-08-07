#ifndef CARDMOVEMENTCOMPONENT_HPP
#define CARDMOVEMENTCOMPONENT_HPP

#include <UrsineEngine/Component.hpp>
#include <UrsineEngine/GameObject.hpp>
#include <UrsineEngine/Signal.hpp>

#include "CardObject.hpp"

using UrsineCore::Component;

namespace DeckOfIllusions
{
  class CardMovementComponent : public Component
  {
    public:
      CardMovementComponent();

      void Update() override;

      void MoveTo(const glm::vec3& aPosition, double aSpeed);
      void RotateTo(double aDegrees,
                    const glm::vec3& aAxis,
                    double aSpeed);

    private:
      glm::vec3 mTargetPosition;

      double mTargetRotation;
      double mCurrentRotation;
      glm::vec3 mRotationAxis;

      double mMoveSpeed;
      double mRotationSpeed;

      bool mMoving;
      bool mRotating;
  };
}

#endif
