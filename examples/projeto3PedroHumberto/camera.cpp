#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

void Camera::computeProjectionMatrix(glm::vec2 const &size) {
  m_projMatrix = glm::mat4(1.0f);
  auto const aspect{size.x / size.y};
  m_projMatrix = glm::perspective(glm::radians(70.0f), aspect, 0.1f, 10.0f);
}

void Camera::computeViewMatrix() {
  m_viewMatrix = glm::lookAt(m_eye, m_at, m_up);
}

void Camera::dolly(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(m_at - m_eye)};

  // Move eye and center forward (speed > 0) or backward (speed < 0)
  m_eye += forward * speed;
  m_at += forward * speed;

  computeViewMatrix();
}

void Camera::truck(float speed) {
  // Compute forward vector (view direction)
  auto const forward{glm::normalize(m_at - m_eye)};
  // Compute vector to the left
  auto const left{glm::cross(m_up, forward)};

  // Move eye and center to the left (speed < 0) or to the right (speed > 0)
  m_at -= left * speed;
  m_eye -= left * speed;

  computeViewMatrix();
}

void Camera::pan(float speed) {
  glm::mat4 transform{1.0f};

  // Rotate camera around its local y axis
  transform = glm::translate(transform, m_eye);
  transform = glm::rotate(transform, -speed, m_up);
  transform = glm::translate(transform, -m_eye);

  m_at = transform * glm::vec4(m_at, 1.0f);

  computeViewMatrix();
}

void Camera::rotateAroundVectorZ(float speed) {
  // Compute a vector that points along the camera's local Z-axis
  auto const forward{glm::normalize(m_at - m_eye)};

  glm::mat4 transform{1.0f};
  // Rotate camera around its local z axis
  transform = glm::rotate(transform, -speed, forward);
  m_up = glm::vec3(transform * glm::vec4(m_up, 0.0f));

  // Ensure the up vector is normalized
  m_up = glm::normalize(m_up);

  computeViewMatrix();
}

void Camera::rotateAroundVectorX(float speed) {
  auto const forward{glm::normalize(m_at - m_eye)};
  glm::vec3 left{glm::cross(m_up, forward)};

  // Rotate the view vector around the left vector
  glm::mat4 transformAt{1.0f};
  transformAt = glm::rotate(transformAt, -speed, left);
  m_at = m_eye + glm::vec3(transformAt * glm::vec4(m_at - m_eye, 0.0f));

  // Rotate the up vector around the left vector as well
  glm::mat4 transformUp{1.0f};
  transformUp = glm::rotate(transformUp, -speed, left);
  m_up = glm::vec3(transformUp * glm::vec4(m_up, 0.0f));

  computeViewMatrix();
}

void Camera::resetPosition() {
  m_eye = {0.0f, 0.5f, 2.5f}; // Camera initial position
  m_at = {0.0f, 0.5f, 0.0f};  // Look-at initial point
  m_up = {0.0f, 1.0f, 0.0f};  // Initial "up" direction
}

void Camera::moveY(float speed) {
  m_eye += m_up * speed;
  m_at += m_up * speed;
  computeViewMatrix();
}