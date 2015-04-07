#include "kmGUIManager.h"

#include "gl_core_4_4.h"

void kmGUIObject::Update(float deltaTime)
{
	if (active && visible)
	{
		// Update Stuff
	}
}

void kmGUIObject::Draw() {
	if (visible) {
		glBindVertexArray(this->vertexdata.m_VAO);
		glDrawElements(GL_TRIANGLES, this->vertexdata.m_index_count, GL_UNSIGNED_INT, 0);
	}
}





void kmGUIManager::SetTarget(Camera* camera) {
	cameraTarget = camera;
}

void kmGUIManager::Update(float deltaTime) {
	for (unsigned int i = 0; i < guiObjects.size(); ++i) {
		guiObjects[i]->Update(deltaTime);
	}
}

void kmGUIManager::Draw() {
	for (unsigned int i = 0; i < guiObjects.size(); ++i) {
		guiObjects[i]->Draw();
	}
}

void kmGUIManager::Render() {

	// Render All GUI Objects to target Camera's mat4
	Draw();
}