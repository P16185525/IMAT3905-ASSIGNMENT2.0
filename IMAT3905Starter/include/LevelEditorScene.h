#pragma once

#include "Model.h"

#include "GameObject.h"
#include "PlayerCharacter.h"
#include "StaticEnvironmentObject.h"
#include "IEngineCore.h"

#include <string>


// IMGUI

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"


//class Model;
class ModelManager;

class LevelEditorScene
{
private:

	bool m_levelLoaded{ false };
	int m_playerIndex{ 1 };

	GameObject * m_playerBackground;

	std::vector<GameObject*> v_gameObjects;
	ModelManager* m_theModelManager;

	// mouse data - we keep old position (last frame or update) so we can get delta's

	double m_oldMouseX;
	double m_oldMouseY;
	double m_mouseX;
	double m_mouseY;
	int m_mouseButtons;
	int m_oldMouseButtons;
	bool m_mouseEnabled{ false };

	

public:

	LevelEditorScene(std::string filename, ModelManager* theModelManager, IEngineCore* engineCore);
	~LevelEditorScene();

	void render(IEngineCore* engineCore);

	void update(float dt);

	bool loadLevelJSON(std::string levelJSONFile);
	bool saveLevelJSON(std::string levelJSONFile);

	void createNewObject();

	PlayerCharacter* getPlayer();
	StaticEnvironmentObject* getControlledObject();

	void imGUIRender();

};
