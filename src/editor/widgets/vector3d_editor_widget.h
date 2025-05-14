////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef VECTOR3D_EDITOR_WIDGET_H
#define VECTOR3D_EDITOR_WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QDoubleSpinBox>

class Vector3DEditorWidget final : public QWidget
{
    Q_OBJECT    
public:
    explicit Vector3DEditorWidget(QWidget *parent = nullptr);

    void setVector(const QVector3D& vector);
    inline QDoubleSpinBox *xSpinBox() const { return m_linkedSpinBoxes[0]; }
    inline QDoubleSpinBox *ySpinBox() const { return m_linkedSpinBoxes[1]; }
    inline QDoubleSpinBox *zSpinBox() const { return m_linkedSpinBoxes[2]; }

signals:
    void valuesChanged(double x, double y, double z);

private:
    QDoubleSpinBox *m_linkedSpinBoxes[3];
    QDoubleSpinBox *m_spinBoxes[3];

    QDoubleSpinBox* createSpinBox();
};

#endif // VECTOR3D_EDITOR_WIDGET_H
