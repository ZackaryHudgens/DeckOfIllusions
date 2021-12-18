#include <Environment.hpp>
#include <Scene.hpp>

#include "CameraMovementComponent.hpp"
#include "CardTextBehaviorComponent.hpp"
#include "DeckObject.hpp"
#include "UITextObject.hpp"

using UrsineEngine::Scene;

using DeckOfIllusions::CameraMovementComponent;
using DeckOfIllusions::DeckObject;

int main()
{
  UrsineEngine::GraphicsOptions gOptions;
  gOptions.mTitle = "Deck of Illusions";
  gOptions.mWidth = 1280;
  gOptions.mHeight = 720;
  gOptions.mCursorMode = UrsineEngine::CursorMode::eNORMAL;

  env.Initialize(gOptions);
  Scene scene;

  // Create a DeckObject.
  auto deck = std::make_unique<DeckObject>();
  deck->Translate(glm::vec3(0.0, 0.0, -5.0));
  deck->LoadDeckFromFile("resources/deck.txt");

  // Create a UITextObject and add the card behavior component.
  auto textObj = std::make_unique<DeckOfIllusions::UITextObject>();
  auto behaviorComp = std::make_unique<DeckOfIllusions::CardTextBehaviorComponent>();
  behaviorComp->ObserveDeck(*deck.get());
  textObj->AddComponent(std::move(behaviorComp));

  // Add the movement component to the camera.
  auto cam = dynamic_cast<UrsineEngine::Camera*>(scene.GetObject("Camera"));
  cam->AddComponent(std::make_unique<CameraMovementComponent>());

  auto camComp = cam->GetFirstComponentOfType<DeckOfIllusions::CameraMovementComponent>();
  camComp->FollowDeck(*deck.get());

  scene.AddObject(std::move(deck));
  scene.AddObject(std::move(textObj));

  env.LoadScene(scene);
  env.Run();

  return 0;
}
