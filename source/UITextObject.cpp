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

  std::string vertexFile = "resources/shaders/TextShader.vert";
  std::string fragmentFile = "resources/shaders/TextShader.frag";
  UrsineEngine::Shader textShader(vertexFile, fragmentFile);
  textShader.Activate();
  textShader.SetFloat("transparency", 1.0);
  textComp->AddShader("textShader", textShader);
  textComp->SetCurrentShader("textShader");

  AddComponent(std::move(textComp));
}
