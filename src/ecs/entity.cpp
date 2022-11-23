#include "ecs/entity.h"

Entity::Entity(const char* name)
{
    m_name = name;
    m_components = std::vector<Component*>();
    this->AddComponent(new Transform(this));
}

Entity::~Entity()
{
    for (auto component : m_components)
    {
        delete component;
    }

    for (auto child : m_children)
    {
        delete child;
    }
}

void Entity::Init()
{
    for (auto component : m_components)
    {
        component->Init();
    }

    for (auto child : m_children)
    {
        child->Init();
    }
}

void Entity::Update(float dt)
{
    for (auto component : m_components)
    {
        component->Update(dt);
    }

    for (auto child : m_children)
    {
        child->Update(dt);
    }
}

void Entity::Render()
{
    for (auto component : m_components)
    {
        component->Render();
    }

    for (auto child : m_children)
    {
        child->Render();
    }
}

Entity* Entity::Clone()
{
    Entity* clone = new Entity(m_name);

    for (auto component : m_components)
    {
        if(strcmp(component->GetName(), "Transform") != 0)
        {
            component->SetEntity(clone);
            clone->AddComponent(component->Clone());
        }
    }

    for (auto child : m_children)
    {
        clone->AddChild(child->Clone());
    }

    return clone;
}


void Entity::AddComponent(Component* component)
{
    m_components.push_back(component);
}

void Entity::RemoveComponent(Component* component)
{
    m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

Entity* Entity::AddChild(Entity* child)
{
    child->SetParent(this);
    this->m_children.push_back(child);
    return child;
}

Entity* Entity::AddChild(Entity* child, const char* name)
{
    child->SetParent(this);
    child->m_name = name;
    m_children.push_back(child);
    return child;
}

Entity* Entity::AddChild(const char* name)
{
    Entity* child = new Entity(name);
    child->SetParent(this);
    m_children.push_back(child);
    return child;
}

Entity* Entity::GetChild(const char* name)
{
    for (auto child : m_children)
    {
        if (child->m_name == name)
        {
            return child;
        }
    }
    return nullptr;
}

std::vector<Entity*> Entity::GetChildren()
{
    return m_children;
}

void Entity::RemoveChild(Entity* child)
{
    m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
    child->Destroy();
}

void Entity::RemoveChild(const char* name)
{
    for (auto child : m_children)
    {
        if (child->m_name == name)
        {
            m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
            child->Destroy();
            return;
        }
    }
}

void Entity::SetParent(Entity* parent)
{
    m_parent = parent;
}

Entity* Entity::GetParent()
{
    return m_parent;
}

void Entity::Destroy()
{
    for (auto component : m_components)
    {
        component->Destroy();
    }

    for (auto child : m_children)
    {
        child->Destroy();
    }
}

std::vector<Component*> Entity::GetComponents()
{
    return m_components;
}


const char* Entity::GetName()
{
    return m_name;
}

void Entity::SetName(const char* name)
{
    m_name = name;
}

