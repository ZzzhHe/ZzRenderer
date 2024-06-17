#pragma once
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
const float SENSITIVITY = 1.5f;
const float ZOOM = 45.0f;

class Camera {
public:
	virtual glm::vec3 getCameraPos() const {return glm::vec3(0.0f);};
	virtual glm::mat4 getViewMatrix() const {return glm::mat4(1.0f);};
	virtual glm::mat4 getProjectionMatrix(float aspectRatio) {
        m_aspectRatio = aspectRatio;
        return glm::mat4(1.0f);
	};
	
	virtual void setNearFarPlane(float near, float far) {
		m_near = near;
		m_far = far;
	};
    virtual float getAspectRatio() const {return m_aspectRatio;};
	virtual float getNearPlane() const {return m_near; };
	virtual float getFarPlane() const {return m_far; };
protected:
    float m_aspectRatio;
	float m_near;
	float m_far;
};

// Camera class for FPS-like controls
class MoveCamera : public Camera{
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
	MoveCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
        : Position(position), WorldUp(up), Front(glm::vec3(0.0f, 0.0f, -1.0f)), Yaw(yaw),
            Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        updateCameraVectors();
    }

    void processKeyboard(Camera_Movement direction, float deltaTime);

    void processMouseMovement(float xoffset, float yoffset);
	
	glm::vec3 getCameraPos() const override{
		return Position;
	}

    glm::mat4 getViewMatrix() const override{
        return glm::lookAt(Position, Position + Front, Up);
    }
    glm::mat4 getProjectionMatrix(float aspectRatio) override{
		m_aspectRatio = aspectRatio;
        return glm::perspective(glm::radians(Zoom), aspectRatio, m_near, m_far);
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler angles
    void updateCameraVectors();
};

// OrbitCamera class for orbit controls around a target
class OrbitCamera : public Camera{
public:
    glm::vec3 Target;       
    float Distance;         
    float Yaw;              
    float Pitch;            
    float MouseSensitivity; 
    float ZoomSensitivity;  

    OrbitCamera(glm::vec3 target = glm::vec3(0.0f), float distance = 15.0f, float yaw = 50.0f, float pitch = 0.0f)
        : Target(target), Distance(distance), Yaw(yaw), Pitch(pitch), MouseSensitivity(0.1f), ZoomSensitivity(0.1f) {}

	glm::vec3 getCameraPos() const override {
		return calculatePosition();
	}
	
    glm::mat4 getViewMatrix() const override {
        glm::vec3 position = calculatePosition();
        return glm::lookAt(position, Target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::mat4 getProjectionMatrix(float aspectRatio) override{
		m_aspectRatio = aspectRatio;
        return glm::perspective(glm::radians(45.0f), aspectRatio, m_near, m_far);
    }

    void processMouseMovement(float xoffset, float yoffset);

    void processMouseScroll(float yoffset);

private:
    glm::vec3 calculatePosition() const;
};
