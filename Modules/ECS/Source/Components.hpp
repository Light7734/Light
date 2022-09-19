#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <string>

namespace Light {

	struct TagComponent
	{
		std::string tag = "New Entity";

		TagComponent()                    = default;
		TagComponent(const TagComponent&) = default;

		TagComponent(const std::string& _tag)
		    : tag(_tag)
		{
		}

		operator std::string_view() { return tag; }

		operator std::string() { return tag; }
		operator const std::string&() const { return tag; }
	};

	struct TransformComponent
	{
		glm::vec3 translation;
		glm::vec3 scale;
		glm::vec3 rotation;

		TransformComponent(const TransformComponent&) = default;

		TransformComponent(const glm::vec3& _translation = glm::vec3(0.0f, 0.0f, 0.0f),
		                   const glm::vec3& _scale       = glm::vec3(1.0f, 1.0f, 1.0f),
		                   const glm::vec3& _rotation    = glm::vec3(0.0f, 0.0f, 0.0f))

		    : translation(_translation), scale(_scale), rotation(_rotation)
		{
		}

		inline glm::mat4 GetTransform() const { return glm::translate(translation) * glm::rotate(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(scale); }

		operator const glm::mat4() const { return GetTransform(); }
	};

} // namespace Light
