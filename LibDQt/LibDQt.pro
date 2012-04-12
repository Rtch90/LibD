CONFIG -= qt
LIBS += -lGL \
    -lSDL \
    -lSDL_ttf \
    -lSDL_image \
    -lSDL_gfx \
    -ltinyxml \
    -lGLU \
	-lz \
	-ltinyxml
HEADERS += ../src/Actor/Player.h \
    ../src/Global/Globals.h \
    ../src/IO/Input.h \
    ../src/Main/Game.h \
    ../src/Main/LGLXWindow.h \
    ../src/Math/Timer.h \
    ../src/Math/MathBox.h \
    ../src/Math/FPS.h \
    ../src/Math/Vec2.h \
    ../src/Sprite/Sprite.h \
    ../src/System/Debug.h \
    ../src/System/ResourceManager.h \
    ../src/Texture/Texture.h \
    ../src/Global/Constants.h \
    ../src/Collision/AABB.h \
    ../src/Level/Level.h \
    ../src/Level/MapTile.h \
    ../src/Level/Map.h \
    ../src/Level/Layer.h \
    ../src/Level/Tileset.h \
    ../src/Level/Layer.h \
    ../src/TMXParser/base64.h \
	../src/TMXParser/Tmx.h \
    ../src/TMXParser/TmxImage.h \
    ../src/TMXParser/TmxLayer.h \
    ../src/TMXParser/TmxMap.h \
	../src/TMXParser/TmxMapTile.h \
    ../src/TMXParser/TmxObject.h \
    ../src/TMXParser/TmxObjectGroup.h \
	../src/TMXParser/TmxPoint.h \
    ../src/TMXParser/TmxPolygon.h \
    ../src/TMXParser/TmxPolyline.h \
    ../src/TMXParser/TmxPropertySet.h \
    ../src/TMXParser/TmxTile.h \
    ../src/TMXParser/TmxTileset.h \
    ../src/TMXParser/TmxUtil.h
SOURCES += ../src/Actor/Player.cpp \
    ../src/Global/Globals.cpp \
    ../src/IO/Input.cpp \
    ../src/Main/main.cpp \
    ../src/Main/LGLXWindow.cpp \
    ../src/Main/Game.cpp \
    ../src/Math/Vec2.cpp \
    ../src/Math/Timer.cpp \
    ../src/Math/FPS.cpp \
    ../src/Sprite/Sprite.cpp \
    ../src/System/Debug.cpp \
    ../src/Texture/Texture.cpp \
    ../src/Collision/AABB.cpp \
    ../src/Level/Level.h \
    ../src/Level/MapTile.h \
    ../src/Level/Map.h \
    ../src/Level/Layer.h \
    ../src/Level/Tileset.h \
    ../src/Level/Layer.h \
    ../src/TMXParser/base64.cpp \
    ../src/TMXParser/TmxImage.cpp \
    ../src/TMXParser/TmxLayer.cpp \
    ../src/TMXParser/TmxMap.cpp \
    ../src/TMXParser/TmxObject.cpp \
    ../src/TMXParser/TmxObjectGroup.cpp \
    ../src/TMXParser/TmxPolygon.cpp \
    ../src/TMXParser/TmxPolyline.cpp \
    ../src/TMXParser/TmxPropertySet.cpp \
    ../src/TMXParser/TmxTile.cpp \
    ../src/TMXParser/TmxTileset.cpp \
    ../src/TMXParser/TmxUtil.cpp
OTHER_FILES += 
