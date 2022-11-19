#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H
#include "ecs/component.h"
#include "components/transform.h"

class EventListener : public Component
{
public:
    EventListener(Transform* transform, glm::vec2* mousePos, bool* mouseLeft, bool* mouseRight, bool* keys);
    virtual ~EventListener();

    void Update(float dt) override;
    EventListener* Clone() override;

    bool isLeftClicked();
    bool isRightClicked();
    bool isLeftClickedandHeld();
    bool isRightClickedandHeld();
    bool isHovered();

private:
    Transform *m_transform;
    glm::vec2 *m_mousePos;
    bool *m_mouseLeft;
    bool *m_mouseRight;
    bool *m_keys;
    float m_leftClickTime, m_rightClickTime;
    bool m_leftClickHeld;
    bool m_rightClickHeld;
    
    bool isMouseOver();

};

#endif