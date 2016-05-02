//
//  scene.cpp
//  CG_P1
//
//  Created by  on 30.04.16.
//  Copyright © 2016 hsos. All rights reserved.
//

#include "../Header/scene.h"

Scene::Scene() {
	this->m_Root.setName("Root");
}

Scene::~Scene() {
}

bool Scene::addSceneFile(const char* scenefile) {
	std::ifstream fileStream(scenefile);
	if (!fileStream) {
		std::cout << "Die Datei \"" << scenefile << "\" kann nicht geoeffnet werden." << std::endl;
		exit(-1);
	}

	// todo

	return true;
}

void Scene::draw() {
	std::set<SceneNode *>::iterator it;

	for (it = this->m_Root.getChildren().begin(); it != this->m_Root.getChildren().end(); ++it) {
		this->draw(*it);
	}
}

void Scene::draw(SceneNode *node) {
	// todo
}