#include "CardMovementComponent.hpp"

#include <iostream>

using DeckOfIllusions::CardMovementComponent;

CardMovementComponent::CardMovementComponent()
  : Component()
  , mTargetPosition(0.0, 0.0, 0.0)
  , mTargetRotation(0.0)
  , mCurrentRotation(0.0)
  , mRotationAxis(0.0, 0.0, 0.0)
  , mMoveSpeed(0.0)
  , mRotationSpeed(0.0)
  , mMoving(false)
  , mRotating(false)
{
}

void CardMovementComponent::Update()
{
  /**
   * Handle any card movement.
   */
  if(mMoving)
  {
    auto vec = glm::mix(GetParent()->GetPosition(), mTargetPosition, mMoveSpeed);

    if(fabs(vec.x - mTargetPosition.x) < 0.005 &&
       fabs(vec.y - mTargetPosition.y) < 0.005 &&
       fabs(vec.z - mTargetPosition.z) < 0.005)
    {
      std::cout << "finished moving" << std::endl;
      // Finalize the translation.
      GetParent()->SetPosition(mTargetPosition);

      // Use the signal to notify everyone that we've finished moving.
      CardObject* cardObj = dynamic_cast<CardObject*>(GetParent());
      CardFinishedMoving.Notify(cardObj);

      mMoving = false;
    }
    else
    {
      GetParent()->SetPosition(vec);
    }
  }

  /**
   * Handle any card rotation.
   */
  if(mRotating)
  {
    if(fabs(mTargetRotation - mCurrentRotation) < 0.5)
    {
      std::cout << "finished rotating" << std::endl;
      // Finalize the rotation.
      GetParent()->Rotate(mTargetRotation - mCurrentRotation, mRotationAxis);

      // Use the signal to notify everyone that we've finished rotating.
      CardObject* cardObj = dynamic_cast<CardObject*>(GetParent());
      CardFinishedRotating.Notify(cardObj);

      mCurrentRotation = 0.0;
      mRotating = false;
    }
    else
    {
      double temp = glm::mix(mCurrentRotation,
                             mTargetRotation,
                             mRotationSpeed);

      GetParent()->Rotate(temp - mCurrentRotation, mRotationAxis);
      mCurrentRotation = temp;
    }
  }
}

void CardMovementComponent::MoveTo(const glm::vec3& aPosition, double aSpeed)
{
  mTargetPosition = aPosition;
  mMoveSpeed = aSpeed;
  mMoving = true;
}

void CardMovementComponent::RotateTo(double aDegrees,
                                     const glm::vec3& aAxis,
                                     double aSpeed)
{
  mTargetRotation = aDegrees;
  mRotationAxis = aAxis;
  mRotationSpeed = aSpeed;
  mRotating = true;
}
