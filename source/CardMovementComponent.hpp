#ifndef CARDMOVEMENTCOMPONENT_HPP
#define CARDMOVEMENTCOMPONENT_HPP

#include <Component.hpp>
#include <GameObject.hpp>
#include <Signal.hpp>

#include "CardObject.hpp"

namespace DeckOfIllusions
{
  class CardMovementComponent : public UrsineEngine::Component
  {
    public:

      /**
       * Constructor.
       */
      CardMovementComponent();

      /**
       * Updates the component.
       *
       * @param aTime The start time of the current Scene's Update().
       */
      void Update(double aTime) override;

      /**
       * Moves the parent GameObject to the given position at the given speed.
       * Once the object has finished moving, the CardFinishedMoving signal is
       * notified in Update(). Movement is interpolated, so the object will
       * slow down as it approaches its destination.
       *
       * @param aPosition The position in world space to move to.
       * @param aSpeed The speed at which to move.
       */
      void MoveTo(const glm::vec3& aPosition,
                  double aSpeed);

      /**
       * Rotates the parent GameObject on the given axis at the given speed.
       * Once the object has finished rotating, the CardFinishedRotating signal
       * is notified in Update(). Rotation is interpolated, so the object will
       * slow down as it approaches its destination.
       *
       * @param aDegrees The amount to rotate in degrees.
       * @param aAxis The axis in model space on which to rotate.
       * @param aSpeed The speed at which to move.
       */
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
