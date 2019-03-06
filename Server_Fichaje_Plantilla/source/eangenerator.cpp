#include "eangenerator.h"
#include <QDebug>

const QString EanGenerator::GS1_PREFIX{"123"};
const QString EanGenerator::MANUFACTURER_CODE{"45"};

QString EanGenerator::calculateCheckSum(QString eanCode) {
    int sum{0};

    for (int i = 0; i < eanCode.length(); i++) {
        sum += eanCode.mid(i, 1).toInt() * ((i%2 == 0) ? 1 : 3);
    }
    int mod = sum%10;
    return QString::number((mod == 0) ? 0 : 10 - mod);
}

QString EanGenerator::getEanCode(QString productCode) {
    QString eanCode{""};

    int sizeProductCode = 13 - (GS1_PREFIX.length() + MANUFACTURER_CODE.length() + 1);

    int offset = sizeProductCode - productCode.length();

    if (offset > 0) {
        for (int i = 0; i < offset; i++) {
            productCode.prepend("0");
        }
    } else if (offset < 0) {
        return eanCode;
    }

    eanCode = GS1_PREFIX + MANUFACTURER_CODE + productCode;
    eanCode += calculateCheckSum(eanCode);

    return eanCode;
}
