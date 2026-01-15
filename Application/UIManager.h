#pragma once

#include <2d/Object/Object2D.h>

#include <vector>
#include <string>
#include <memory>
#include <map>

class UIManager {

public:

	static UIManager* GetInstance();

	void Initialize();

	void Update();

	void Draw();

	void CreateUI(const std::string& groupName, const std::string& spriteName);

private:

	std::map<std::string, std::vector<std::unique_ptr<Object2D>>> ui;

public:

	std::vector<Object2D*> GetUiGroup(const std::string& groupName);

};