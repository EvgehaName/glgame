////////////////////////////////////////////////////////////////////////////
//	Created 	: 10.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#ifndef INSPECTOR_WIDGET_H
#define INSPECTOR_WIDGET_H

#include <qtreewidget.h>
#include <qheaderview.h>
#include <qvector3d.h>
#include <qspinbox.h>

// Forward declaration
class GameObject;
class Vector3DEditorWidget;

class InspectorWidget final : public QTreeWidget
{
    Q_OBJECT
public:
    InspectorWidget(QWidget* parent = nullptr);
    void init(QTreeWidget* sceneTreeWidget);

private:
    QTreeWidget* m_treeScene;

    void destroy();
    void setupGeneral();
    void createInspector(GameObject* gameObject);

    Vector3DEditorWidget* create3DVectorEditor(QTreeWidgetItem* root, const QString& nodeName);
    QDoubleSpinBox* createQDoubleSpinBox();

private slots:
    void sceneTreeSelectionChanged();

    // QTreeView interface
protected:
    void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
    void drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const;
};

#endif // INSPECTOR_WIDGET_H
