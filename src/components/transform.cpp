#include "components/transform.h"
#include <iostream>

Transform::Transform(Entity* entity)
{
    m_entity = entity;
    m_position = glm::vec2(200.0f, 200.0f);
    m_rotation = 0.0f;
    m_scale = glm::vec2(100.0f, 100.0f);
    m_name = "Transform";
}

Transform::~Transform()
{
}

void Transform::Init()
{
}

void Transform::Update(float dt)
{
}

void Transform::Destroy()
{
}

void Transform::SetPosition(glm::vec2 position)
{
    m_position = position;
}

void Transform::SetPosition(float x, float y)
{
    m_position = glm::vec2(x, y);
}

void Transform::SetRotation(float rotation)
{
    m_rotation = rotation;
}

void Transform::SetScale(glm::vec2 scale)
{
    m_scale = scale;
}

void Transform::SetScale(float x, float y)
{
    m_scale = glm::vec2(x, y);
}

glm::vec2 Transform::GetPosition()
{
    return m_position;
}

float Transform::GetRotation()
{
    return m_rotation;
}

glm::vec2 Transform::GetScale()
{
    return m_scale;
}