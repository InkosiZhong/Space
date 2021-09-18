#include "xmlutil.h"

Util::XmlUtil::XmlUtil(QString path, QString symbol)
{
    resetPath(path);
    resetConnSymbol(symbol);
}

void Util::XmlUtil::resetPath(QString path) {
    m_path = path;
}

void Util::XmlUtil::resetConnSymbol(QString symbol) {
    m_conn_symbol = symbol;
}

bool Util::XmlUtil::create(QString path) {
    QDomProcessingInstruction instruction = m_doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    m_doc.appendChild(instruction);
    return true;
}

bool Util::XmlUtil::load(QString path) {
    QFile config_xml(path.isEmpty() ? m_path : path);
    if (!config_xml.open(QIODevice::ReadOnly)) return false;
    bool res = m_doc.setContent(&config_xml);
    config_xml.close();
    return res;
}

bool Util::XmlUtil::save(QString path, uint n_space) {
    QFile config_xml(path.isEmpty() ? m_path : path);
    if(!config_xml.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    QTextStream out(&config_xml);
    m_doc.save(out, n_space);
    config_xml.close();
    return true;
}

bool Util::XmlUtil::append(QString src, QString val) {
    QStringList node_path = src.split(m_conn_symbol);
    QStringList::iterator it = node_path.begin();
    QDomElement elem = m_doc.firstChildElement(*it);
    if (elem.isNull()) {
        elem = m_doc.createElement(*it);
        m_doc.appendChild(elem);
    }
    while (++it != node_path.end()) {
        QDomElement child = elem.firstChildElement(*it);
        if (child.isNull()) {
            child = m_doc.createElement(*it);
            elem.appendChild(child);
        }
        elem = child;
    }
    QDomText text = m_doc.createTextNode(val);
    elem.appendChild(text);
    return true;
}

bool Util::XmlUtil::set(QString src, QString val) {
    QStringList node_path = src.split(m_conn_symbol);
    QStringList::iterator it = node_path.begin();
    QDomElement elem = m_doc.firstChildElement(*it);
    if (elem.isNull())return false;
    while (++it != node_path.end()) {
        elem = elem.firstChildElement(*it);
        if (elem.isNull())return false;
    }
    if (elem.firstChild().isNull())return false;
    elem.firstChild().setNodeValue(val);
    return true;
}

QString Util::XmlUtil::get(QString src) {
    QStringList node_path = src.split(m_conn_symbol);
    QStringList::iterator it = node_path.begin();
    QDomElement elem = m_doc.firstChildElement(*it);
    if (elem.isNull())return "";
    while (++it != node_path.end()) {
        elem = elem.firstChildElement(*it);
        if (elem.isNull())return "";
    }
    return elem.text();
}
