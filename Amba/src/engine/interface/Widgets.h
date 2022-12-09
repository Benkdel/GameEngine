#pragma once

#include <engine/interface/inferface.h>


namespace Amba {

	// layout tools
	void ShowDiagnosticInfo(Scene* scene, glm::vec4& clearColor);
	void ShowActiveEntitiesInScene(Scene* scene);
	void ShowTransform(Scene* scene);
	void ShowAdditionalEntitiesData(Scene* scene);
	void ShowEntityUnderMouse(Scene* scene);
	void ShowCameraSelection(Scene* scene, Camera* activeCamera, bool& editorsCameraActive);

	// basic widgets methods
	void TransformWidget(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.f);
	void SingleFloatWidget(const std::string& label, float& value, float resetValue = 0.0f, float columnWidth = 100.f);


	// method to be used for debugging and
	// show specific data in ImGui
	void DebugingMethods(Scene* scene);

	// debuging info
	void ColliderData(Scene* scene);

}

static std::string VecToStr(glm::vec3 v3)
{
	return ("< " + std::to_string(v3.x) + ", " +
		std::to_string(v3.y) + ", " +
		std::to_string(v3.z) + " >");
}

static std::string VecToStr(glm::vec2 v2)
{
	return ("< " + std::to_string(v2.x) + ", " +
		std::to_string(v2.y) + " >");
}
