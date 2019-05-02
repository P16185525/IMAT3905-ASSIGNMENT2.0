#include "LevelEditorScene.h"

#include "IEngineCore.h"
#include "Model.h"
#include "Game.h"
#include "TransformComponent.h"
#include "ColourComponent.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "ModelManager.h"
#include "JSON\json.h"

#include "BackgroundColourGameObject.h"

#include <fstream>
#include <sstream>

LevelEditorScene::LevelEditorScene(std::string filename, ModelManager * theModelManager, IEngineCore * engineCore)
{
	m_playerBackground = new BackgroundColourGameObject();

	engineCore->getMouseState(m_mouseX, m_mouseY, m_mouseButtons);

	m_oldMouseX = m_mouseX;
	m_oldMouseY = m_mouseY;
	m_oldMouseButtons = m_mouseButtons;
}

LevelEditorScene::~LevelEditorScene()
{

}

void LevelEditorScene::render(IEngineCore* engineCore)
{

}

void LevelEditorScene::update(float dt)
{

}

bool LevelEditorScene::loadLevelJSON(std::string levelJSONFile)
{
	return false;
}

bool LevelEditorScene::saveLevelJSON(std::string levelJSONFile)
{
	return false;
}

void LevelEditorScene::createNewObject()
{

}

PlayerCharacter * LevelEditorScene::getPlayer()
{
	return nullptr;
}

StaticEnvironmentObject * LevelEditorScene::getControlledObject()
{
	return nullptr;
}

void LevelEditorScene::imGUIRender()
{

}
