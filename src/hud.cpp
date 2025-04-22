#include "hud.h"

Hud::Hud(QWidget *parent)
{
    m_ammo    = new UIAmmo(parent);
    m_health  = new UIHealthBar(parent);
    m_status  = new UIDeathmatch(parent);
    m_stamina = new UIStaminaBar(parent);

    m_ammo->show();
    m_health->show();
    m_status->show();
    m_stamina->show();
}

void Hud::onResize(int width, int height)
{
    const int margin  = 10;
    const int spacing = 3;
    const int rightX  = width  - margin;
    const int bottomY = height - margin;

    /* Самый нижний */
    m_stamina->move(
        rightX - m_stamina->width(),
        bottomY - m_stamina->height()
    );

    /* Выше стамины */
    m_health->move(
        rightX - m_health->width(),
        bottomY - m_stamina->height() - m_health->height() - spacing
    );

    /* Выше здоровья */
    m_ammo->move(
        rightX - m_ammo->width(),
        bottomY - m_stamina->height() - m_health->height() - m_ammo->height() - spacing
    );

    // Центр вверху
    m_status->move(
        (width - m_status->width()) / 2,
        margin
    );
}
