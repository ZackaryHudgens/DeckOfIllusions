#include <Environment.hpp>
#include <Scene.hpp>

#include "CardObject.hpp"
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

  env.LoadScene(scene);
  env.Run();

  return 0;
}
