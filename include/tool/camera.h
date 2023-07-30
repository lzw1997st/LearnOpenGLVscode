#ifndef CAMERA_H
#define CAMERA_H

#include <tool/shader.h>

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum Camera_Movement
{
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

class Camera
{
private:
  /* data */
  //
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // euler Angles
  float Yaw;
  float Pitch;

  void updateCameraVectors()
  {
    // calculate the new Front vector
    glm::vec3 front(1.0f);
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up = glm::normalize(glm::cross(this->Right, this->Front));
  }

public:
  // camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;
  Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
         glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                 WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
                                                 MovementSpeed(SPEED),
                                                 MouseSensitivity(SENSITIVITY),
                                                 Zoom(ZOOM)
  {
    this->Position = pos;
    this->Up = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    updateCameraVectors();
  }
  ~Camera()
  {
  }

  glm::mat4 GetViewMatrix()
  {
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
  }

  void ProcessKeyboard(Camera_Movement direction, float deltaTime)
  {
    float velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
      this->Position += this->Front * velocity;
    if (direction == BACKWARD)
      this->Position -= this->Front * velocity;
    if (direction == LEFT)
      this->Position -= this->Right * velocity;
    if (direction == RIGHT)
      this->Position += this->Right * velocity;
  }

  // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
  {
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
      if (this->Pitch > 89.0f)
        this->Pitch = 89.0f;
      if (this->Pitch < -89.0f)
        this->Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
  }

  // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset)
  {
    this->Zoom -= (float)yoffset;
    if (this->Zoom < 1.0f)
      this->Zoom = 1.0f;
    if (this->Zoom > 45.0f)
      this->Zoom = 45.0f;
  }
};

#endif