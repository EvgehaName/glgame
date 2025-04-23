#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "geom.h"

class GameObject : public Geom, protected QOpenGLFunctions_4_3_Core
{
    public:
        GameObject();
        ~GameObject();

        void draw();
    
    private:
        void initTexture();
        void initBuffersPlaceObject();
        QMap<std::string, QOpenGLTexture*> textureMap;

};

#endif // GAMEOBJECT_H
