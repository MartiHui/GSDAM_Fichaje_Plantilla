#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <QString>

struct Registro {
    int empleadoId;
    QString empleadoNombre;
    QString empleadoApellido;
    bool esEntrada;
    QString fecha;
};

#endif // DATASTRUCTS_H
