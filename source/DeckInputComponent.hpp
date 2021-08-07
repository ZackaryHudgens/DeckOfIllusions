#ifndef DECKINPUTCOMPONENT_HPP
#define DECKINPUTCOMPONENT_HPP

#include <UrsineEngine/CoreSignals.hpp>
#include <UrsineEngine/Component.hpp>

namespace DeckOfIllusions
{
  class DeckInputComponent : public UrsineCore::Component
  {
    public:
      DeckInputComponent();

    private:
      void HandleKeyPress(const UrsineCore::KeyCode& aCode,
                          int aMods);
  };
}

#endif
