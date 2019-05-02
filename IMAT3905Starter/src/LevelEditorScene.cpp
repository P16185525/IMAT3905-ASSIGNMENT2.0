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

LevelEditorScene::LevelEditorScene(std::string filename, ModelManager * theModelManager, IEngineCore * engineCore) : m_theModelManager(theModelManager)
{
	m_playerBackground = new BackgroundColourGameObject();
	m_levelLoaded = loadLevelJSON(filename);

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
	PlayerCharacter* playerCharacter = getPlayer();

	// Keeps the old mouse position and button state
	m_oldMouseX = m_mouseX;
	m_oldMouseY = m_mouseY;
	m_oldMouseButtons = m_mouseButtons;

	// Gets the current mouse position and button state
	engineCore->getMouseState(m_mouseX, m_mouseY, m_mouseButtons);	

	double xDelta = m_mouseX - m_oldMouseX;
	double yDelta = m_mouseY - m_oldMouseY;

	if (((m_mouseButtons & 0x2) != 0) & ((m_oldMouseButtons & 0x2) == 0))
	{
		m_mouseEnabled = !m_mouseEnabled;
	}

	if (m_mouseEnabled)
	{
		const float mouseSensitivity = 200.0f;

		float theta = static_cast<float>(xDelta) / mouseSensitivity;
		float thi = static_cast<float>(yDelta) / mouseSensitivity;	

		TransformComponent* transformComponent = playerCharacter->getComponent<TransformComponent>();

		// Applies the transforms seperately to avoid roll
		transformComponent->pitch(thi);
		transformComponent->yaw(theta);

		playerCharacter->SetCameraPositionFromTransformComponent(transformComponent);
	}

	float redValue = 0, greenValue = 0, blueValue = 1;
	if (m_playerBackground->getComponent<RedComponent>())
		redValue = m_playerBackground->getComponent<RedComponent>()->m_colourValue;
	if (m_playerBackground->getComponent<GreenComponent>())
		greenValue = m_playerBackground->getComponent<GreenComponent>()->m_colourValue;
	if (m_playerBackground->getComponent<BlueComponent>())
		blueValue = m_playerBackground->getComponent<BlueComponent>()->m_colourValue;

	engineCore->renderColouredBackground(redValue, greenValue, blueValue);

	// Updates the camera
	engineCore->setCamera(getPlayer()->getComponent<CameraComponent>());

	// Draw the game objects
	for (auto gameObject : v_gameObjects)
	{
		Model* model = gameObject->getComponent<ModelComponent>()->getModel();
		glm::mat4 matrix = gameObject->getComponent<TransformComponent>()->getModelMatrix();
		engineCore->drawModel(model, matrix);

	}
}

void LevelEditorScene::update(float dt)
{

}

bool LevelEditorScene::loadLevelJSON(std::string levelJSONFile)
{
	bool loadOK = true;

	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;

	jsonData.open(levelJSONFile.c_str());
	// check for errors
	if (!reader.parse(jsonData, root))
	{
		std::cout << "Failed to parse data from: "
			<< levelJSONFile
			<< reader.getFormattedErrorMessages();

		loadOK = false;
		return loadOK;
	}
	const Json::Value gameObjects = root["GameObjects"];

	//

	int numberOfCubes = gameObjects.size();
	//v_GameObjects.resize(numberOfCubes);

	// size() tells us how large the array is
	for (int i = 0; i < (int)gameObjects.size(); i++)
	{


		// get string
		std::cout << gameObjects[i]["name"].asString() << " loaded\n";

		// link this to model

		const Json::Value modelNode = gameObjects[i]["model"];

		std::string modelName = modelNode.asString();	// no index as not an array

		Model* model{ nullptr };

		if (modelNode.type() != Json::nullValue)
		{
			model = m_theModelManager->getModel("assets/models/" + modelName);
		}
		else
		{
			model = m_theModelManager->getModel("assets/models/cone.obj");		
		}

		// test for no model later!!!!!

		if (model == nullptr)
		{
			loadOK = false;
		}

		float x, y, z;
		// get the position node
		const Json::Value posNode = gameObjects[i]["position"];
		x = posNode[0].asFloat(); // get float
		y = posNode[1].asFloat();
		z = posNode[2].asFloat();



		glm::vec3 position(x, y, z);

		glm::quat orientation(1.0f, 0.0f, 0.0f, 0.0f);


		const Json::Value orientNode = gameObjects[i]["orientation"];
		if (orientNode.type() != Json::nullValue)
		{

			// get orientation here e.t.c.

		}


		glm::vec3 scale(1.0f, 1.0f, 1.0f);
		const Json::Value scaleNode = gameObjects[i]["scale"];
		if (scaleNode.type() != Json::nullValue)
		{

			// get scale here e.t.c.

		}

		std::cout << x << "," << y << "," << z << std::endl;

		// todo - fix this to be data dependent
		if (i == m_playerIndex)
		{
			v_gameObjects.push_back(new PlayerCharacter(model, position, orientation));
		}
		else
		{
			v_gameObjects.push_back(new StaticEnvironmentObject(model, position, orientation));
		}
	}

	return loadOK;
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
	return (PlayerCharacter*)v_gameObjects[m_playerIndex];
}

StaticEnvironmentObject * LevelEditorScene::getControlledObject()
{
	return nullptr;
}

void LevelEditorScene::imGUIRender()
{
	static bool show_test_window = true;
	static bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);

	// imgui
	ImGui_ImplGlfwGL3_NewFrame();

	// Shows a simple window
	{
		TransformComponent* tc = getPlayer()->getComponent<TransformComponent>();

		float* fpx = &(tc->m_position.x);
		float* fpy = &(tc->m_position.y);
		float* fpz = &(tc->m_position.z);
		//static float f = 0.0f;
		ImGui::Text("Level editor");
		ImGui::SliderFloat("floatx", fpx, 0.0f, 2.0f);
		ImGui::SliderFloat("floaty", fpy, 0.0f, 2.0f);
		ImGui::SliderFloat("floatz", fpz, 0.0f, 2.0f);

		ImGui::ColorEdit3("clear colour", (float*)&clear_color);
		if (ImGui::Button("Test Window")) show_test_window ^= 1;
		if (ImGui::Button("Another Window")) show_another_window ^= 1;
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	// Show another simple window, this time using an explicit Begin/End pair
	if (show_another_window)
	{
		ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Another Window", &show_another_window);
		ImGui::Text("Hello");
		ImGui::End();
	}

	// Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
		ImGui::ShowTestWindow(&show_test_window);
	}

	// Rendering
	//int display_w, display_h;
	//glfwGetFramebufferSize(m_window, &display_w, &display_h);
	//glViewport(0, 0, display_w, display_h);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui::Render();
}
