
#include "styles.h"

Style::Style()
{
    setId(StyleId::Classic);
}

Style& Style::getInstance() {
    static Style instance;
    return instance;
}

void Style::setId(StyleId id) {
    switch (id) {
    case StyleId::Classic: m_state.reset(new StyleClassicState); break;
    case StyleId::Metro  : m_state.reset(new StyleMetroState  ); break;
    }
}

void Style::StyleClassicState::drawRect(QPainter& painter, QRect& rect, qreal roundRadius)
{
    painter.drawRoundedRect(rect, roundRadius, roundRadius);
}

void Style::StyleMetroState::drawRect(QPainter& painter, QRect& rect, qreal )
{
    painter.drawRect(rect);
}
