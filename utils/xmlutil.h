#ifndef XMLUTIL_H
#define XMLUTIL_H

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <map>

namespace Util{

class XmlUtil
{
public:
    XmlUtil(const QString& path = "", const QString& symbol = ".");
    void resetPath(const QString& path);
    void resetConnSymbol(const QString& symbol);

    bool create(const QString& path = "");
    bool load(const QString& path = "");
    bool save(const QString& path = "", uint n_space = 4);

    bool addMapping(const QString& key, const QString& val);
    bool removeMapping(const QString& key);
    bool alterMapping(const QString& key, const QString& val);
    bool getMapping(const QString& key, QString& val);

    /*** append: call when xml is incomplete
     * QString src should be like "A.B.C"
     */
    bool append(const QString& src, const QString& val);

    /*** set: call when the xml is loaded
     * QString src should be like "A.B.C"
     */
    bool set(const QString& src, const QString& val);

    /*** get: call when the xml is loaded
     * QString src should be like "A.B.C"
     */
    QString get(const QString& src, const QString& def_val = "");

private:
    QString m_path = "";
    QString m_conn_symbol = ".";
    QDomDocument m_doc;
    std::map<QString, QString> m_mapping;
};

}

#endif // XMLUTIL_H
