#ifndef HUD_H
#define HUD_H

#include "ui/uiammo.h"
#include "ui/uideathmatch.h"

#include "ui/uihealthbar.h"
#include "ui/uistaminabar.h"

class Hud : public QObject
{
    Q_OBJECT
public:
    Hud(QWidget *parent);
    Q_SLOT void onResize(int width, int height);

private:
    UIAmmo * m_ammo;
    UIHealthBar * m_health;
    UIDeathmatch * m_status;
    UIStaminaBar * m_stamina;
};

#endif // HUD_H
