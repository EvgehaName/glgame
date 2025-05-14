////////////////////////////////////////////////////////////////////////////
//	Created 	: 10.05.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
///////////////////////////////////////////////////////////////////////////
#include "inspector_widget.h"
#include "game/gameobject.h"

#include "widgets/vector3d_editor_widget.h"

#include <qpainter.h>
#include <qapplication.h>

InspectorWidget::InspectorWidget(QWidget* parent)
    : QTreeWidget(parent)
    , m_treeScene(nullptr)
{   
    header()->hide();
    setIndentation(8);
    setRootIsDecorated(true);
    setIconSize(QSize(12, 12));
    setFocusPolicy(Qt::NoFocus);
    //setAlternatingRowColors(true);
    setHeaderLabels(QStringList() << "" << "");
    setSelectionMode(QAbstractItemView::NoSelection);
}

void InspectorWidget::init(QTreeWidget *sceneTreeWidget)
{
    if (!sceneTreeWidget) {
        return;
    }

    /* Подключимся к сигналу itemSelectionChanged(), чтобы знать когда пользователь выбрал объект в дереве */
    m_treeScene = sceneTreeWidget;
    connect(m_treeScene, &QTreeWidget::itemSelectionChanged, this, &InspectorWidget::sceneTreeSelectionChanged);
}

void InspectorWidget::setupGeneral()
{

}

void InspectorWidget::createInspector(GameObject *gameObject)
{
    /* Удалим старые виджеты */
    clear();

    /* Настройки самого QTreeWidget */
    setColumnCount(2);

    auto general = new QTreeWidgetItem(this);
    general->setExpanded(true);
    general->setText(0, "general");
    addTopLevelItem(general);

    auto pos = create3DVectorEditor(general, "position");
    pos->setVector(gameObject->getPosition());
    connect(pos, &Vector3DEditorWidget::valuesChanged, this, [gameObject, this](double x, double y, double z){
        gameObject->setPosition(QVector3D(x, y, z));
    });

    auto rot = create3DVectorEditor(general, "rotation");
    rot->setVector(gameObject->getRotation());
    connect(rot, &Vector3DEditorWidget::valuesChanged, this, [gameObject, this](double x, double y, double z){
        gameObject->setRotation(QVector3D(x, y, z));
    });

    auto scl = create3DVectorEditor(general, "scale");
    scl->setVector(gameObject->getScale());
    connect(scl, &Vector3DEditorWidget::valuesChanged, this, [gameObject, this](double x, double y, double z){
        gameObject->setScale(QVector3D(x, y, z));
    });
}

Vector3DEditorWidget *InspectorWidget::create3DVectorEditor(QTreeWidgetItem* root, const QString &nodeName)
{
    auto editorItem = new QTreeWidgetItem(root);
    editorItem->setText(0, nodeName);
    addTopLevelItem(editorItem);

    auto vector = new Vector3DEditorWidget;
    setItemWidget(editorItem, 1, vector);

    auto xItem = new QTreeWidgetItem(editorItem);
    auto yItem = new QTreeWidgetItem(editorItem);
    auto zItem = new QTreeWidgetItem(editorItem);

    xItem->setText(0, "x");    
    yItem->setText(0, "y");
    zItem->setText(0, "z");

    setItemWidget(xItem, 1, vector->xSpinBox());
    setItemWidget(yItem, 1, vector->ySpinBox());
    setItemWidget(zItem, 1, vector->zSpinBox());

    return vector;
}

void InspectorWidget::sceneTreeSelectionChanged()
{
    if (!m_treeScene) {
        return;
    }

    /* Получим указатель на выбранный элемент дерева */
    QTreeWidgetItem* treeItemSelected = m_treeScene->currentItem();
    if (!treeItemSelected) {
        return;
    }

    /* Получим указатель на игровой объект привязанный к элементу дерева */
    GameObject* treeGameObject = treeItemSelected->data(0, Qt::UserRole).value<GameObject*>();
    if (!treeGameObject) {
        return;
    }

    /* Создадим UI инспектора для редактирования объекта */
    createInspector(treeGameObject);
}

void InspectorWidget::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    /* Просто для удобства обращения, переменная с более коротким именем */
    QStyleOptionViewItem opt = options;

    /* Область заливки */
    QRect colorRect = opt.rect;
    colorRect.setLeft(0);

    /* Цвет заливки */
    const auto color = QColor(0xc0cfe3);

    /* Рисуем горизонтальную линию под строкой */
    painter->setPen(QPen(color, 1, Qt::SolidLine));
    painter->drawLine(opt.rect.bottomLeft(), opt.rect.bottomRight());

    /* Рисуем вертикальные линии между колонками */
    for (int col = 0; col < model()->columnCount(); ++col) {
        QRect rect = visualRect(model()->index(index.row(), col, index.parent()));
        painter->drawLine(rect.topRight(), rect.bottomRight());
    }

    if (!index.parent().isValid())
    {
        /* Выделяем жирным категории */
        opt.font.setBold(true);

        /* Выравнивание текста категории по центру */
        auto item = itemFromIndex(index);
        item->setTextAlignment(0, Qt::AlignHCenter | Qt::AlignCenter);

        /* Заливаем всю строку */
        painter->fillRect(colorRect, color);
    }
    else
    {
        /* Заливаем только отступ от левого края */
        colorRect.setRight(indentation() * 2);
        painter->fillRect(colorRect, color);
    }

    // Отрисовываем строку стандартным способом
    QTreeWidget::drawRow(painter, opt, index);
}

void InspectorWidget::drawBranches(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    /* Фикс, чтобы кнопки раскрытия узла всегда было выровнены */
    QRect myRect = rect;
    myRect.setRight(12);

    // Отрисовываем стандартным способом
    QTreeWidget::drawBranches(painter, myRect, index);
}
