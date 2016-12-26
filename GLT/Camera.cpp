#include "Camera.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <iostream>

using namespace glt;

Camera* Camera::active = nullptr;

Camera::Camera( vec2 screenSize ) {
    if (active == nullptr)
        use( );

    this->screenSize = screenSize;
}

void Camera::checkDirty( ) {
    if (position_old != position ||
         direction_old != direction ||
         fov_old != fieldOfView ||
         near_old != nearPlane ||
         far_old != farPlane ||
         perspective_old != perspective ||
         orthoFrustum_old != orthoFrustum ||
         screenSize_old != screenSize) {
        isDirty = true;
    }
}

void Camera::clean( ) {
    position_old = position;
    direction_old = direction;

    fov_old = fieldOfView;
    near_old = nearPlane;
    far_old = farPlane;

    perspective_old = perspective;
    orthoFrustum_old = orthoFrustum;

    screenSize_old = screenSize;

    isDirty = false;
}

mat4 Camera::getMatrix( ) {
    checkDirty( );

    if (!isDirty)
        return cameraMatrix;

    clean( );

    mat4 projection;

    if (perspective)
        projection = glm::perspective( radians( fieldOfView ), getAspectRatio( ), nearPlane, farPlane );
    else
        projection = glm::ortho( -getAspectRatio( ) * orthoFrustum * 0.5f, getAspectRatio( ) * orthoFrustum * 0.5f, -orthoFrustum * 0.5f, orthoFrustum * 0.5f, nearPlane, farPlane );

    mat4 view = glm::lookAt( position, position + direction, up );

    cameraMatrix = projection * view;
    return cameraMatrix;
}

Ray Camera::screenToWorld( vec2 screen ) {
    screen		= clamp( screen, vec2( 0.f ), screenSize );
    screen		= (screen / screenSize - vec2( 0.5f )) * vec2( 2.f, -2.f );

    mat4 projView = getMatrix( );
    projView = inverse( projView );

    // Near
    vec4 worldNear = projView * vec4( screen.x, screen.y, -1, 1 );
    worldNear = worldNear / worldNear.w;

    vec4 worldFar = projView * vec4( screen.x, screen.y, 1, 1 );
    worldFar = worldFar / worldFar.w;

    return Ray::fromTo( vec3( worldNear ), vec3( worldFar ) );
}

vec2 Camera::worldToScreen( vec3 world ) {
    return vec2( getMatrix( ) * vec4( world, 1.0 ) );
}

void Camera::use( ) {
    Camera::active = this;
}