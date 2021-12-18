#include "UITextObject.hpp"

#include <TextComponent.hpp>

using DeckOfIllusions::UITextObject;

/******************************************************************************/
UITextObject::UITextObject(const std::string& aName)
  : GameObject(aName)
{
  auto textComp = std::make_unique<UrsineEngine::TextComponent>();
  textComp->LoadFont("resources/fonts/BitterPro-Regular.ttf");
  textComp->SetFont("Bitter Pro", "Regular");
  textComp->SetCoordinateSystem(UrsineEngine::CoordinateSystem::eSCREEN_SPACE);
  textComp->SetSize(36);

  AddComponent(std::move(textComp));
}
