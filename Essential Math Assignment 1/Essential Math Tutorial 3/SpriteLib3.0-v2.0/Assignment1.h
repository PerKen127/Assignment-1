#pragma once

#include "Scene.h"

class Assignment1 : public Scene
{
public:
	Assignment1(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;

protected:

};