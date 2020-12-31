#pragma once

#include "Scene.h"
#include "Camera.h"
#include "global.h"

class SceneManager
{
	static SceneManager* __instance;
	Scene* currentScene;
	Scene* holderScene;
public:
	SceneManager();
	~SceneManager();

	static SceneManager* GetInstance();
	void SetScene(Scene* scene) { currentScene = scene; }
	Scene* GetScene() { return currentScene; }

	void SetHolderScene(Scene* scene) { holderScene = scene; }
	Scene* GetHolderScene() { return holderScene; }

	void Load(LPCWSTR path);
	void Unload();
	void Update(DWORD dt);
	void Render();
};


