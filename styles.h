#ifndef STYLES_H
#define STYLES_H

#include <QString>

enum class Style {Classic,
                  Metro};
struct Styles
{
    static QString classic;
    static QString metro;

    static Style   defaultStyle;

    static QString resolve(Style   style);
    static Style   resolve(QString style);
};

#endif // STYLES_H
