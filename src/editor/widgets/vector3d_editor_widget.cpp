////////////////////////////////////////////////////////////////////////////
//	Created 	: 13.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "vector3d_editor_widget.h"
#include <QVector3D>

constexpr double MAX_DOUBLE = std::numeric_limits<double>::max();

Vector3DEditorWidget::Vector3DEditorWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    for (int i = 0; i < 3; i++)
    {
        auto *spinBox = createSpinBox();
        m_spinBoxes[i] = spinBox;
        layout->addWidget(spinBox);

        auto* linkedSpinBox = createSpinBox();
        m_linkedSpinBoxes[i] = linkedSpinBox;

        connect(spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [i, this]() {
            m_linkedSpinBoxes[i]->setValue(m_spinBoxes[i]->value());
            emit valuesChanged(m_spinBoxes[0]->value(), m_spinBoxes[1]->value(), m_spinBoxes[2]->value());
        });

        connect(linkedSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [i, this]() {
            m_spinBoxes[i]->setValue(m_linkedSpinBoxes[i]->value());
        });
    }
}

void Vector3DEditorWidget::setVector(const QVector3D &vector)
{
    m_spinBoxes[0]->setValue(vector.x());
    m_spinBoxes[1]->setValue(vector.y());
    m_spinBoxes[2]->setValue(vector.z());
}

QDoubleSpinBox *Vector3DEditorWidget::createSpinBox()
{
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(this);
    spinBox->setStyleSheet("QDoubleSpinBox { border: none; background: transparent; }");
    spinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    spinBox->setRange(-MAX_DOUBLE, MAX_DOUBLE);
    spinBox->setSingleStep(0.1);
    spinBox->setDecimals(4);

    return spinBox;
}
