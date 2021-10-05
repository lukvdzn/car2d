#include <iostream>
#include <fstream>
#include "LevelEditor.h"
#include "VectorMath.h"

LevelEditor::LevelEditor() 
	: track_outline{ sf::LinesStrip }, 
	track_inline{ sf::LinesStrip }, 
    track_checkpoints{ sf::Lines },
	done_outline{ false },
    done_inline{ false },
    done_checkpoints{ false },
    mouse_pressed {false}
{

}

void LevelEditor::draw(sf::RenderWindow& window)
{
    update(window);
    window.draw(track_outline);
    window.draw(track_inline);
    window.draw(track_checkpoints);
}

void LevelEditor::update(sf::RenderWindow& window)
{
    using namespace sf;
    static Vector2f first_mouse_pos = { -1, -1 };

    // only save map if key 's' is pressed and track has been drawn
    if (done_inline && Keyboard::isKeyPressed(Keyboard::S))
    {
        done_checkpoints = true;
        return;
    }
    
    auto& va = !done_outline ? track_outline : (!done_inline ? track_inline : track_checkpoints);
    auto mouse_pos = Vector2f{ Mouse::getPosition(window) };
    auto l_last_vertex_pos = [&]() -> Vector2f& { return va[va.getVertexCount() - 1].position; };

    if (!Mouse::isButtonPressed(Mouse::Button::Left)) mouse_pressed = false;
    
    if (!done_outline || !done_inline)
    {
        if (!mouse_pressed && Mouse::isButtonPressed(Mouse::Button::Left))
        {
            mouse_pressed = true;
            if (first_mouse_pos == Vector2f{ -1, -1 })
            {
                first_mouse_pos = Vector2f{ mouse_pos };
                va.append(Vertex{ mouse_pos });
                va.append(Vertex{ mouse_pos });
            }
            else {
                // if more than 1 vertex exists, close track polygon if 
                // current mouse position is close to first set vertex
                if (va.getVertexCount() > 1 && v_magnitude(mouse_pos - first_mouse_pos) < 20.f)
                {
                    if (done_outline) done_inline = true;
                    done_outline = true;
                    l_last_vertex_pos() = first_mouse_pos;
                    first_mouse_pos = { -1, -1 };
                    return;
                }
                else {
                    l_last_vertex_pos() = mouse_pos;
                    va.append(Vertex{ mouse_pos });
                }
            }
        }
        if (va.getVertexCount() > 0) l_last_vertex_pos() = mouse_pos;
    }
    else {
        if (!done_checkpoints)
        {
            static bool point_set = false;
            if (!mouse_pressed)
            {
                if (Mouse::isButtonPressed(Mouse::Button::Left))
                {
                    mouse_pressed = true;
                    if (point_set)
                    {
                        l_last_vertex_pos() = mouse_pos;
                        point_set = false;
                    }
                    else {
                        va.append(Vertex{ mouse_pos });
                        va.append(Vertex{ mouse_pos });
                        point_set = true;
                    }
                }
                else {
                    if (point_set) l_last_vertex_pos() = mouse_pos;
                }
            }
        }
    }
}

bool LevelEditor::done_with_edit() const
{
    return done_checkpoints;
}

void LevelEditor::save_to_file(const char* file_name) const
{
    std::ofstream f_out{ file_name };
    if (!f_out)
    {
        std::cout << "file could not be opened!" << std::endl;
        return;
    }
    auto l_write_va = [&](const sf::VertexArray& va) {
        f_out << va.getVertexCount() << std::endl;
        for (auto i = 0; i < va.getVertexCount(); ++i)
        {
            const auto& p = va[i].position;
            f_out << p.x << " " << p.y << std::endl;
        }
    };

    l_write_va(track_outline);
    l_write_va(track_inline);
    l_write_va(track_checkpoints);

    std::cout << "map " << file_name << " save successful!" << std::endl;
}