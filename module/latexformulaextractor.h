#ifndef LATEXFORMULAEXTRACTOR_H
#define LATEXFORMULAEXTRACTOR_H

#include <QString>
#include <list>
#include <string>
#include <utils/curlutil.h>
#include <utils/jsonutil.h>
#include <QDebug>

#include "constants.h"

extern enum LatexType latexType;

struct LatexInfoPack {
    int m_type = LATEX_UNKOWN;
    double m_rotate_degrees = 0;
    double m_rotate_confidence = 0;
    double m_confidence = 0; // set as -1 as error
    QString m_msg = "";

    LatexInfoPack(const Json::Value& root);
    LatexInfoPack();
};

class LatexFormulaExtractor
{
public:
    LatexFormulaExtractor(const QString& app_id = "",
                          const QString& app_key = "");

public:
    /** upload the image from disk
     * @brief handle
     * @param path
     * @return
     */
    LatexInfoPack handle(const QString& path);

    void setAppIdAndKey(const QString& app_id = "",
                        const QString& app_key = "");

    bool available();

private:
    QString m_app_id;
    QString m_app_key;

};

#endif // LATEXFORMULAEXTRACTOR_H
