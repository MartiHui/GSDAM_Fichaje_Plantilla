#ifndef EANGENERATOR_H
#define EANGENERATOR_H

#include <QString>

class EanGenerator {
private:
    const static QString GS1_PREFIX;
    const static QString MANUFACTURER_CODE;
    EanGenerator() = delete;
    EanGenerator(const EanGenerator &copy) = delete;
    EanGenerator& operator=(const EanGenerator &copy) = delete;
    static QString calculateCheckSum(QString eanCode);

public:
    static QString getEanCode(QString productCode);
};

#endif // EANGENERATOR_H
