#ifndef UITEXTOBJECT_HPP
#define UITEXTOBJECT_HPP

#include "GameObject.hpp"

namespace DeckOfIllusions
{
  class UITextObject : public UrsineEngine::GameObject
  {
    public:

      /**
       * Constructor.
       */
      UITextObject(const std::string& aName = "Text");
  };
}

#endif
