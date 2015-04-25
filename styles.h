#ifndef STYLES_H
#define STYLES_H

#include <QString>
#include <QPainter>
#include <memory>

enum class StyleId : int {Classic,
                          Metro};

inline int     resolveStyle(StyleId style)
{
    return static_cast<int>(style);
}

inline StyleId resolveStyle(int     style)
{
    return static_cast<StyleId>(style);
}

class Style
{
    Style();
    Style(const Style&);
    Style& operator=(const Style&);

public:
    static Style&  getInstance();

    inline void drawRect(QPainter& painter, QRect& rect, qreal roundRadius);

           void    setId(StyleId id);
    inline void    setId(int id    );
    inline StyleId id   () const;
    inline int     idInt() const;

private:
    class State {
    public:
        State(StyleId id) : m_id(id) {}
        virtual ~State() {}
        virtual void drawRect(QPainter& painter, QRect& rect, qreal roundRadius) = 0;
        const StyleId m_id;
    };
    class StyleClassicState : public Style::State {
    public:
        StyleClassicState() : State(StyleId::Classic){}

        void drawRect(QPainter& painter, QRect& rect, qreal roundRadius);
    };
    class StyleMetroState : public Style::State {
    public:
        StyleMetroState() : State(StyleId::Metro){}

        void drawRect(QPainter& painter, QRect& rect, qreal );
    };

    typedef std::shared_ptr<State> SpState;
    SpState m_state;
};

inline void Style::drawRect(QPainter& painter, QRect& rect, qreal roundRadius) {
    m_state->drawRect(painter, rect, roundRadius);
}

inline void Style::setId(int id) {
    setId(resolveStyle(id));
}

inline StyleId Style::id() const {
    return m_state->m_id;
}

inline int Style::idInt() const {
    return resolveStyle(m_state->m_id);
}

#endif // STYLES_H
