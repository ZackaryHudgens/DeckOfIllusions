#ifndef DECKBEHAVIORCOMPONENT_HPP
#define DECKBEHAVIORCOMPONENT_HPP

#include <CoreSignals.hpp>
#include <Component.hpp>

namespace DeckOfIllusions
{
  class DeckBehaviorComponent : public UrsineEngine::Component
  {
    public:

      /**
       * Constructor.
       */
      DeckBehaviorComponent();

      /**
       * Updates the component.
       */
      void Update() override;

    private:
      enum class State
      {
        eIDLE,
        eWAITING_FOR_FLIP,
        eWAITING_FOR_FADE,
        eNO_INPUT
      };

      /**
       * A handler function that gets called whenever a key is pressed.
       *
       * @param aCode The key that was pressed.
       * @param aMods Any modifiers present when the key was pressed.
       */
      void HandleKeyPress(const UrsineEngine::KeyCode& aCode,
                          int aMods);

      State mState;
  };
}

#endif
