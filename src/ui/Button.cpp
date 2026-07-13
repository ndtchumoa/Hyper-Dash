#include "ui/Button.h"

bool Button::init(
    SDL_Renderer*      renderer,
    TTF_Font*          font,
    const std::string& text,
    int                x,
    int                y,
    int                paddingX,
    int                paddingY)
{
    m_paddingX = paddingX;
    m_paddingY = paddingY;
    m_hovered  = false;
    m_clicked  = false;

    if (!m_label.init(renderer, font, text, kColorNormal))
        return false;

    m_label.setPosition(x + paddingX, y + paddingY);

    updateHitbox();

    // Đặt lại x, y hitbox về vị trí thực (trước padding)
    m_hitbox.x = x;
    m_hitbox.y = y;

    return true;
}

void Button::handleEvent(const SDL_Event& event)
{
    switch (event.type)
    {
    case SDL_MOUSEMOTION:
    {
        const SDL_Point mouse = {
            event.motion.x,
            event.motion.y
        };

        m_hovered = SDL_PointInRect(&mouse, &m_hitbox);
        break;
    }

    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            const SDL_Point mouse = {
                event.button.x,
                event.button.y
            };

            if (SDL_PointInRect(&mouse, &m_hitbox))
                m_clicked = true;
        }
        break;

    default:
        break;
    }
}

void Button::render(SDL_Renderer* renderer) const
{
    // Vẽ background hitbox khi hover
    if (m_hovered)
    {
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 200);
        SDL_RenderFillRect(renderer, &m_hitbox);
    }

    // Border luôn hiển thị
    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
    SDL_RenderDrawRect(renderer, &m_hitbox);

    m_label.render(renderer);
}

void Button::clean()
{
    m_label.clean();
}

bool Button::isClicked() const
{
    return m_clicked;
}

bool Button::isHovered() const
{
    return m_hovered;
}

void Button::resetClick()
{
    m_clicked = false;
}

SDL_Rect Button::getBounds() const
{
    return m_hitbox;
}

void Button::updateHitbox()
{
    const SDL_Rect labelBounds = m_label.getBounds();

    m_hitbox =
    {
        labelBounds.x - m_paddingX,
        labelBounds.y - m_paddingY,
        labelBounds.w + m_paddingX * 2,
        labelBounds.h + m_paddingY * 2
    };
}