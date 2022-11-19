#include "components/event_listener.h"
#include "ecs/entity.h"

EventListener::EventListener(Transform* transform, glm::vec2* mousePos, bool* mouseLeft, bool* mouseRight, bool* keys)
{
    m_transform = transform;
    m_mousePos = mousePos;
    m_mouseLeft = mouseLeft;
    m_mouseRight = mouseRight;
    m_keys = keys;
    m_leftClickTime = 0.0f;
    m_rightClickTime = 0.0f;
    m_name = "EventListener";
}

EventListener::~EventListener()
{
}

void EventListener::Update(float dt)
{
    if (m_leftClickTime > 0.0f)
    {
        if(m_leftClickTime > dt)
        {
            m_leftClickTime -= dt;
        }
        else
        {
            m_leftClickTime = 0.0f;
        }
        
    } 

    if (m_rightClickTime > 0.0f)
    {
        if(m_rightClickTime > dt)
        {
            m_rightClickTime -= dt;
        }
        else
        {
            m_rightClickTime = 0.0f;
        }
        
    }

    if (!*m_mouseLeft)
    {
        m_leftClickHeld = false;
    }

    if (!*m_mouseRight)
    {
        m_rightClickHeld = false;
    }
}

EventListener* EventListener::Clone()
{
    EventListener* eventListener = new EventListener(m_entity->GetComponent<Transform>(), m_mousePos, m_mouseLeft, m_mouseRight, m_keys);
    return eventListener;
}

bool EventListener::isLeftClickedandHeld()
{
    if (isMouseOver() && *m_mouseLeft)
    {
        return true;
    }
    return false;   
}

bool EventListener::isRightClickedandHeld()
{
    if(isMouseOver() && *m_mouseRight)
    {
        return true;
    }
    return false;
}

bool EventListener::isHovered()
{
    return isMouseOver();
}

bool EventListener::isLeftClicked()
{
    if (isMouseOver() && *m_mouseLeft && m_leftClickTime == 0.0f && !m_leftClickHeld)
    {
        m_leftClickTime = 0.1f;
        m_leftClickHeld = true;
        return true;
    }
    return false;
}

bool EventListener::isRightClicked()
{
    if (isMouseOver() && *m_mouseRight && m_rightClickTime == 0.0f && !m_rightClickHeld)
    {
        m_rightClickTime = 0.1f;
        m_rightClickHeld = true;
        return true;
    }
    return false;
}

bool EventListener::isMouseOver()
{
    //the top left corner is (0,0)
    //the bottom right corner is (m_game->Width, m_game->Height)
    //the mouse position is (m_game->MousePos.x, m_game->MousePos.y)
    //the entity position is (m_transform->GetPosition().x, m_transform->GetPosition().y)
    //the entity size is (m_transform->GetScale().x, m_transform->GetScale().y)

    //check if the mouse is over the entity
    if (m_mousePos->x >= m_transform->GetPosition().x - m_transform->GetScale().x / 2 &&
        m_mousePos->x <= m_transform->GetPosition().x + m_transform->GetScale().x / 2 &&
        m_mousePos->y >= m_transform->GetPosition().y - m_transform->GetScale().y / 2 &&
        m_mousePos->y <= m_transform->GetPosition().y + m_transform->GetScale().y / 2)
    {
        return true;
    }
    return false;
}



