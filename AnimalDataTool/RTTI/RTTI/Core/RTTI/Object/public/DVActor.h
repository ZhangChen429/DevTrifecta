#pragma once
#include "DVObject.h"

class DVActor: public DVObject
{
    DECAVERSE_TYPE(DVActor, DVObject)
    DVREFLECT_BEGIN(DVActor)
            DVPROPERTY(float, health, m_health)
            DVPROPERTY(float, speed, m_speed)
            DVFUNCTION_1(TakeDamage, &DVActor::TakeDamage)
            DVFUNCTION_1(MoveTo, &DVActor::Move)
        DVREFLECT_END()
public:
    
    DVActor() : m_health(100.0f), m_speed(5.0f)
    {
        m_name = "Actor";
    }

    float GetHealth() const { return m_health; }
    void SetHealth(float health) { m_health = health; }

    float GetSpeed() const { return m_speed; }
    void SetSpeed(float speed) { m_speed = speed; }

    void TakeDamage(float damage)
    {
        m_health -= damage;
        std::cout << m_name << " took " << damage << " damage. Health: " << m_health << '\n';
    }

    void Move(float distance)
    {
        std::cout << m_name << " moved " << distance << " units at speed " << m_speed << '\n';
    }

    void Update() override
    {
        std::cout << "Actor::Update() - " << m_name << " (Health: " << m_health << ")" << '\n';
    }

private:
    float m_health;
    float m_speed;
};
