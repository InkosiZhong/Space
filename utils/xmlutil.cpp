#include "xmlutil.h"

Util::XmlUtil::XmlUtil(const QString& path, const QString& symbol)
{
    resetPath(path);
    resetConnSymbol(symbol);
}

void Util::XmlUtil::resetPath(const QString& path) {
    m_path = path;
}

void Util::XmlUtil::resetConnSymbol(const QString& symbol) {
    m_conn_symbol = symbol;
}

bool Util::XmlUtil::create(const QString& path) {
    if (!path.isEmpty())resetPath(path);
    QDomProcessingInstruction instruction = m_doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    m_doc.appendChild(instruction);
    return true;
}

bool Util::XmlUtil::load(const QString& path) {
    QFile config_xml(path.isEmpty() ? m_path : path);
    if (!config_xml.open(QIODevice::ReadOnly)) return false;
    bool res = m_doc.setContent(&config_xml);
    config_xml.close();
    return res;
}

bool Util::XmlUtil::save(const QString& path, uint n_space) {
    QFile config_xml(path.isEmpty() ? m_path : path);
    if(!config_xml.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    QTextStream out(&config_xml);
    m_doc.save(out, n_space);
    config_xml.close();
    return true;
}

bool Util::XmlUtil::addMapping(const QString& key, const QString& val){
    if (m_mapping.find(key) == m_mapping.end()){
        m_mapping[key] = val;
        return true;
    }
    return false;
}

bool Util::XmlUtil::removeMapping(const QString& key){
    if (m_mapping.find(key) != m_mapping.end()){
        m_mapping.erase(key);
        return true;
    }
    return false;
}

bool Util::XmlUtil::alterMapping(const QString& key, const QString& val, bool check){
    if (!check || m_mapping.find(key) != m_mapping.end()){
        m_mapping[key] = val;
        return true;
    }
    return false;
}

bool Util::XmlUtil::getMapping(const QString& key, QString& val){
    if (m_mapping.find(key) != m_mapping.end()){
        val = m_mapping[key];
        return true;
    }
    return false;
}

bool Util::XmlUtil::append(const QString& src, const QString& val) {
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
    QDomText text = m_doc.createTextNode(m_mapping[val].isEmpty() ? val : m_mapping[val]);
    elem.appendChild(text);
    return true;
}

bool Util::XmlUtil::set(const QString& src, const QString& val) {
    QStringList node_path = src.split(m_conn_symbol);
    QStringList::iterator it = node_path.begin();
    QDomElement elem = m_doc.firstChildElement(*it);
    if (elem.isNull())return false;
    while (++it != node_path.end()) {
        elem = elem.firstChildElement(*it);
        if (elem.isNull())return false;
    }
    if (elem.firstChild().isNull())return false;
    elem.firstChild().setNodeValue(m_mapping[val].isEmpty() ? val : m_mapping[val]);
    return true;
}

QString Util::XmlUtil::get(const QString& src, const QString& def_val) {
    QStringList node_path = src.split(m_conn_symbol);
    QStringList::iterator it = node_path.begin();
    QDomElement elem = m_doc.firstChildElement(*it);
    if (elem.isNull())return def_val;
    while (++it != node_path.end()) {
        elem = elem.firstChildElement(*it);
        if (elem.isNull())return def_val;
    }
    QString val = elem.text();
    return m_mapping.find(val) == m_mapping.end() ? val : m_mapping[val];
}
