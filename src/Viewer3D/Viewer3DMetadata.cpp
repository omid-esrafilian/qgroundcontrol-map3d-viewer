#include "Viewer3DMetadata.h"


#define ELEMENT_APP_SETTING                         "AppSetting"
#define ATTR_APP_CITY_FILE_PATH                     "CityMapFilePath"
#define ATTR_APP_HEIGHT_BIAS                        "HeightBias"

#define TAG_APP_CITY_FILE_PATH                      "path"
#define TAG_APP_HEIGHT_BIAS                         "height"


Viewer3DMetadata::Viewer3DMetadata(const char *data_file_path, QObject *parent)
    : QObject{parent}
{
    metadata_file_path = QString(data_file_path);
}

bool Viewer3DMetadata::loadMetaDataFile()
{
    QDomDocument document;
    QFile xmlFile(metadata_file_path);
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        // Error while loading file
        qDebug() << "Cannot load the 3D viewer meta data file !";
        return 0;
    }
    QByteArray file_contetnt = xmlFile.readAll();
    xmlFile.close();

    document.setContent(file_contetnt);
    QDomElement root = document.documentElement();
    QDomElement node = root.firstChild().toElement();

    while(node.isNull() == false)
    {
        if(node.tagName() == ATTR_APP_CITY_FILE_PATH)
            meta_data.city_map_path = node.attribute(TAG_APP_CITY_FILE_PATH,"");

        if(node.tagName() == ATTR_APP_HEIGHT_BIAS)
            meta_data.height_bias = node.attribute(TAG_APP_HEIGHT_BIAS,"0").toDouble();

        node = node.nextSibling().toElement();
    }

    qDebug() << meta_data.city_map_path << meta_data.height_bias;

    return 1;
}

bool Viewer3DMetadata::updateMetaDataFile()
{
    QFile xmlFile(metadata_file_path);
    if (!xmlFile.open(QFile::WriteOnly | QFile::Text ))
    {
        qDebug() << "Cannot access the 3D viewer meta data XML file!";
        xmlFile.close();
        return 0;
    }
    QTextStream xmlContent(&xmlFile);

    QDomDocument document;

    //make the root element
    QDomElement root = document.createElement(ELEMENT_APP_SETTING);
    //add it to document
    document.appendChild(root);

    QDomElement city_map_path = document.createElement(ATTR_APP_CITY_FILE_PATH);
    city_map_path.setAttribute(TAG_APP_CITY_FILE_PATH, meta_data.city_map_path);
    root.appendChild(city_map_path);

    QDomElement height_bias = document.createElement(ATTR_APP_HEIGHT_BIAS);
    height_bias.setAttribute(TAG_APP_HEIGHT_BIAS, QString::number(meta_data.height_bias, 'f', 2));
    root.appendChild(height_bias);


    xmlContent << document.toString();

    return 1;
}
