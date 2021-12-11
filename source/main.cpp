#include <Environment.hpp>
#include <Scene.hpp>

#include "DeckObject.hpp"

using UrsineEngine::Scene;

using DeckOfIllusions::DeckObject;

int main()
{
  UrsineEngine::GraphicsOptions gOptions;
  gOptions.mTitle = "Deck of Illusions";
  gOptions.mWidth = 800;
  gOptions.mHeight = 600;
  gOptions.mCursorMode = UrsineEngine::CursorMode::eNORMAL;

  env.Initialize(gOptions);
  Scene scene;

  // Create a DeckObject.
  auto deck = std::make_unique<DeckObject>();
  deck->Translate(glm::vec3(0.0, 0.0, -5.0));
  deck->LoadDeckFromFile("resources/deck.txt");

  scene.AddObject(std::move(deck));

  auto deck2 = std::make_unique<DeckObject>("Deck_2");
  deck2->Translate(glm::vec3(2.0, 0.0, -5.0));
  deck2->LoadDeckFromFile("resources/deck.txt");

  scene.AddObject(std::move(deck2));

  auto cam = dynamic_cast<UrsineEngine::Camera*>(scene.GetObject("Camera"));
  cam->Translate(glm::vec3(0.0, 5.0, 0.0));
  cam->Rotate(-25, glm::vec3(1.0, 0.0, 0.0));

  env.LoadScene(scene);
  env.Run();

  return 0;
}
