
#include "styles.h"

QString Styles::classic      = "Classic";
QString Styles::metro        = "Metro";
Style   Styles::defaultStyle = Style::Classic;

QString Styles::resolve(Style style)
{
    if      (style == Style::Classic)
        return Styles::classic;
    else if (style == Style::Metro  )
        return Styles::metro;
}

Style Styles::resolve(QString style)
{
    if      (style == Styles::classic)
        return Style::Classic;
    else if (style == Styles::metro  )
        return Style::Metro;
}
