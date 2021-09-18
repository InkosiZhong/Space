#ifndef XMLUTIL_H
#define XMLUTIL_H

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Util{

class XmlUtil
{
public:
    XmlUtil(QString path = "", QString symbol = ".");
    void resetPath(QString path);
    void resetConnSymbol(QString symbol);

    bool create(QString path = "");
    bool load(QString path = "");
    bool save(QString path = "", uint n_space = 4);

    /*** append: call when xml is incomplete
     * QString src should be like "A.B.C"
     */
    bool append(QString src, QString val);

    /*** set: call when the xml is loaded
     * QString src should be like "A.B.C"
     */
    bool set(QString src, QString val);

    /*** get: call when the xml is loaded
     * QString src should be like "A.B.C"
     */
    QString get(QString src);

private:
    QString m_path = "";
    QString m_conn_symbol = ".";
    QDomDocument m_doc;
};

}

#endif // XMLUTIL_H
