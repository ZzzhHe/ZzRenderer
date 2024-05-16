#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

// Define the camera movement directions
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

// Camera class for FPS-like controls
class Camera {
public:
    // Camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : Position(position), WorldUp(up), Front(glm::vec3(0.0f, 0.0f, -1.0f)), Yaw(yaw), 
            Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        updateCameraVectors();
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }
    glm::mat4 GetProjectionMatrix(float aspectRatio) {
        return glm::perspective(glm::radians(Zoom), aspectRatio, 0.1f, 100.0f);
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler angles
    void updateCameraVectors();
};

// OrbitCamera class for orbit controls around a target
class OrbitCamera {
public:
    glm::vec3 Target;       
    float Distance;         
    float Yaw;              
    float Pitch;            
    float MouseSensitivity; 
    float ZoomSensitivity;  

    OrbitCamera(glm::vec3 target = glm::vec3(0.0f), float distance = 5.0f, float yaw = -90.0f, float pitch = 0.0f)
        : Target(target), Distance(distance), Yaw(yaw), Pitch(pitch), MouseSensitivity(0.1f), ZoomSensitivity(0.1f) {}

    glm::mat4 GetViewMatrix() const {
        glm::vec3 position = CalculatePosition();
        return glm::lookAt(position, Target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 GetProjectionMatrix(float aspectRatio, float fov = 45.0f, float nearClip = 0.1f, float farClip = 100.0f) const {
        return glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    }

    void ProcessMouseMovement(float xoffset, float yoffset);

    void ProcessMouseScroll(float yoffset);

private:
    glm::vec3 CalculatePosition() const;
};