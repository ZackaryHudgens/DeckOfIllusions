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
  deck->LoadDeckFromFile("resources/deck.txt");

  // Create a UITextObject and add the card behavior component.
  auto textObj = std::make_unique<DeckOfIllusions::UITextObject>();
  auto behaviorComp = std::make_unique<DeckOfIllusions::CardTextBehaviorComponent>();
  textObj->AddComponent(std::move(behaviorComp));
  textObj->SetPosition(glm::vec3(0.0,
                                 70.0,
                                 0.0));

  auto comp = textObj->GetFirstComponentOfType<DeckOfIllusions::CardTextBehaviorComponent>();
  comp->ObserveDeck(*deck.get());

  // Add the movement component to the camera.
  auto cam = scene.GetDefaultCamera();
  cam->AddComponent(std::make_unique<CameraMovementComponent>());

  auto camComp = cam->GetFirstComponentOfType<DeckOfIllusions::CameraMovementComponent>();
  camComp->FollowDeck(*deck.get());

  scene.AddObject(std::move(deck));
  scene.AddObject(std::move(textObj));

  env.LoadScene(scene);
  env.Run();

  return 0;
}
