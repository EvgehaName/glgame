////////////////////////////////////////////////////////////////////////////
//	Created 	: 27.04.2025
//	Author		: Denis Eremenko
//	Copyright (C) Denis Eremenko - 2025
////////////////////////////////////////////////////////////////////////////
#include "level_loader.h"

#include <qfile.h>
#include <qjsonvalue.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsondocument.h>

#include "application.h"
#include "game/level.h"
#include "game/plain.h"
#include "dynamics/physics_world.h"
#include "dynamics/collision/collision_box.h"

constexpr const char* supported_doc_version = "0.1";

bool LevelLoader::load(const QString& filepath, Level* out)
{
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open level file:" << filepath;
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qWarning() << "Incorrect level format";
        return false;
    }

    QJsonObject root = doc.object();

    QJsonObject metadata = root["metadata"].toObject();
    if (metadata["version"].toString() != supported_doc_version) {
        qWarning() << "Unsupported format level";
        return false;
    }

    QJsonArray objectsArray = root["objects"].toArray();
    for (const QJsonValue& objectValue : objectsArray)
    {
        QJsonObject object = objectValue.toObject();

        QJsonArray position = object["position"].toArray();
        QJsonArray rotation = object["rotation"].toArray();
        QJsonArray scale    = object["scale"].toArray();

        GameObject* objPtr = new Plain(position, rotation, scale);
        out->addGameObject(objPtr);
    }

    PhysicsWorld& phWorld = PhysicsWorld::getInstance();

    QJsonArray collisionArray = root["collision_objects"].toArray();
    for (const QJsonValue& collisionValue : collisionArray)
    {
        QJsonObject collisionObject = collisionValue.toObject();

        QJsonArray sizeArray = collisionObject["size"].toArray();

        QJsonArray collision_position_array = collisionObject["position"].toArray();
        QJsonArray col_rot = collisionObject["rotation"].toArray();
        QJsonArray col_scl = collisionObject["scale"].toArray();

        CollisionBox* collision_object_ptr = new CollisionBox(phWorld.getSpace(), sizeArray);
        collision_object_ptr->setPosition(collision_position_array);
        out->add_collision_object(collision_object_ptr);
    }
    return true;
}
