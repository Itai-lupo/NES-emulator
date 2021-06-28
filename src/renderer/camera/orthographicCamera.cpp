#include "orthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "logger.h"

namespace LaughTaleEngine
{
    orthographicCamera::orthographicCamera(float left, float right, float bottom, float top)
		: projectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), viewMatrix(1.0f)
	{
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}
	
	void orthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void orthographicCamera::recalculateViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
        glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));

		viewMatrix = glm::inverse(transform);
		viewProjectionMatrix = projectionMatrix * viewMatrix;
	}

	void orthographicCamera::SetPosition(const glm::vec3& position) 
	{
		this->position = position; 
		recalculateViewMatrix(); 
	}

	void orthographicCamera::setRotation(float rotation) 
	{ 
		this->rotation = rotation; 
		recalculateViewMatrix(); 
	}
}