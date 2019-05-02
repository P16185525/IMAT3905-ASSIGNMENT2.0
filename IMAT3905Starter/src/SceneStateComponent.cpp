#pragma once
#include "SceneStateComponent.h"


SceneStateComponent::SceneStateComponent()
{
	//m_sceneIndex = 0;

	m_levelEditorSceneIndex = 0;
}

SceneStateComponent::~SceneStateComponent()
{

}

void SceneStateComponent::OnUpdate(float dt)
{

}
void SceneStateComponent::OnMessage(const std::string msg)
{
	if (msg == "Level1")
	{
		SetSceneIndex(0);
	}
	else if (msg == "Level2")
	{
		SetSceneIndex(1);
	}

}

int SceneStateComponent::GetSceneIndex()
{
	//return m_sceneIndex;

	return m_levelEditorSceneIndex;
}
void SceneStateComponent::SetSceneIndex(int newSceneIndex)
{
	//m_sceneIndex = newSceneIndex;

	m_levelEditorSceneIndex = newSceneIndex;
}

