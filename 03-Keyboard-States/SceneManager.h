#pragma once

#include "Scene.h"
#include "Camera.h"
#include "global.h"

class SceneManager
{
	static SceneManager* __instance;
	Scene* currentScene;
public:
	SceneManager();
	~SceneManager();

	static SceneManager* GetInstance();
	void SetScene(Scene* scene) { currentScene = scene; }
	Scene* GetScene() { return currentScene; }

	void Load(LPCWSTR path);
	void Unload();
	void Update(DWORD dt);
	void Render();
};


