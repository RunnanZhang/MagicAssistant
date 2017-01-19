#ifndef MAGICASSISTANT_SETTINGS_H
#define MAGICASSISTANT_SETTINGS_H

#include <QObject>
#include <QVariant>
#include <QMap>

class Settings : public QObject
{
    Q_OBJECT

public:
    Settings(const QString &filePath, QObject *parent = Q_NULLPTR);
    ~Settings();

    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;

    void insert(const QString &key, const QVariant &value);
    QList<QVariant> values(const QString &key) const;

    /// @brief parser functions.
    static QString variantToString(const QVariant &v);
    static QVariant stringToVariant(const QString &s);

private:
    /// @brief
    bool readXmlFile(const QString &fileName);
    bool writeXmlFile(const QString &fileName);

    static QStringList splitArgs(const QString &s, int idx);

    /// @brief make sure dir exist. make sure this file is the type of xml.
    QString processFile(const QString &filePath);

    /// @brief 检查文件类型，若以非suffix后缀结尾，则添加此后缀至file.
    void checkFileFormat(QString &file, const QString &suffix);

private:
    QMap<QString, QVariant> m_map;
    QString m_fileName;

    // write file after setValue call.
    bool m_isWrite;

};

#endif // MAGICASSISTANT_SETTINGS_H
