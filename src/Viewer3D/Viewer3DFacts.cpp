#include "Viewer3DFacts.h"
#include "QGCApplication.h"


Viewer3DFacts::Viewer3DFacts()
{
    if(qgcApp()->viewer3D())
    {
        _osmParser = qgcApp()->viewer3D()->osmParser();
        _qmlBackend = qgcApp()->viewer3D()->qmlBackend();
    }
}
