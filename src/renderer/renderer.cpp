#include "renderer.h"
#include "glad/glad.h"
#include "math/constants.h"
#include "renderer/passes/renderPassViewDefault.h"
#include "renderer/passes/renderPassViewWireframe.h"
#include "renderer/tasks/task.h"

using namespace LRender;

const float Renderer::PROJECTION_ANGLE = Constants::PI * 0.25f;
const float Renderer::Z_NEAR = 0.05f;
const float Renderer::Z_FAR = 400;
const Vector Renderer::CLEAR_COLOR = Vector(0.3f, 0.4f, 0.6f);

// OpenGL debugging:
#include <iostream>

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	if(type != GL_DEBUG_TYPE_ERROR)
		return;

	std::cout << message << std::endl;
}
//

Renderer::Renderer(const size_t width, const size_t height) :
	updatePass(std::make_shared<RenderPassViewDefault>()) {
	glEnable(GL_DEPTH_TEST);
	glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, 1);

	// OpenGL debugging:
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);
	//

	setSize(width, height);
	updateProjection();
}

void Renderer::enqueue(const std::shared_ptr<Task>& task) {
	std::lock_guard<std::mutex> lock(access);

	tasks.push_back(task);
}

void Renderer::update() {
	std::lock_guard<std::mutex> lock(access);

	for(auto &task : tasks)
		task->perform(*this);

	tasks.clear();
}

void Renderer::render() const {
	updatePass->render(shaders, orbit, projection, terrains, agents);
}

void Renderer::center() {
	orbit.setFocus(sceneCenter);
}

void Renderer::setSize(const size_t width, const size_t height) {
	glViewport(0, 0, width, height);

	aspect = float(width) / height;

	updateProjection();
}

void Renderer::mouseMove(const size_t x, const size_t y) {
	orbit.mouseMove(x, y);
}

void Renderer::mousePress(const MouseButton button) {
	switch(button) {
	case DRAG:
		orbit.mouseGrabDrag();

		break;
	case PAN:
		orbit.mouseGrabPan();
		
		break;
	}
}

void Renderer::mouseRelease(const MouseButton button) {
	switch(button) {
	case DRAG:
		orbit.mouseReleaseDrag();

		break;
	case PAN:
		orbit.mouseReleasePan();

		break;
	}
}

void Renderer::scrollUp() {
	orbit.zoomIn();
}

void Renderer::scrollDown() {
	orbit.zoomOut();
}

void Renderer::setMode(const Mode mode) {
	switch(mode) {
	case DEFAULT:
		updatePass = std::make_shared<RenderPassViewDefault>();

		break;
	case WIREFRAME:
		updatePass = std::make_shared<RenderPassViewWireframe>();

		break;
	}
}

void Renderer::loadScene(const Scene *scene, Report *report) {
	terrains.clear();
	agents.clear();

	terrains.push_back(TerrainModel(scene->getTerrain()));

	for(auto &agent : scene->getAgents()) {
		agents.push_back(AgentModel(agent, randomizer));

		if(report) {
			const auto model = --agents.end();

			report->add(ReportAgent(
				ReportLimits(model->getMinimum(), model->getMaximum()),
				ReportArea(model->getArea())
			));
		}
	}

	// Calculate center
	sceneCenter.x = scene->getTerrain().getWidth() * 0.5f;
	sceneCenter.z = scene->getTerrain().getHeight() * 0.5f;

	if(report->hasLimits())
		sceneCenter.y = report->getLimits().getMinimum().y +
		(report->getLimits().getMaximum().y - report->getLimits().getMinimum().y) * 0.5f;
	else
		sceneCenter.y = 0;
}

void Renderer::updateProjection() {
	projection = Matrix::makePerspective(
		PROJECTION_ANGLE,
		aspect,
		Z_NEAR,
		Z_FAR);
}