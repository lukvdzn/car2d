#include <iostream>
#include <fstream>
#include "LevelEditor.h"
#include "VectorMath.h"

LevelEditor::LevelEditor() 
	: track_outline{ sf::LinesStrip }, 
	track_inline{ sf::LinesStrip }, 
	finished_track_outline{ false },
    finished_both{ false }
{

}

void LevelEditor::draw(sf::RenderWindow& window)
{
    update(window, finished_track_outline ? track_inline : track_outline);
    window.draw(track_outline);
    window.draw(track_inline);
}
// TODO fix duplicate points in vertexArrays 
void LevelEditor::update(sf::RenderWindow& window, sf::VertexArray& track)
{
    using namespace sf;
    static Vector2i prev_mouse_pos = { -1, -1 };
    static Vector2f first_mouse_pos = { -1, -1 };
    static char state = 0;

    if (Mouse::isButtonPressed(Mouse::Button::Left) && Mouse::getPosition(window) != prev_mouse_pos)
    {
        state = 1;
        prev_mouse_pos = Mouse::getPosition(window);
        if (first_mouse_pos == Vector2f{ -1, -1 })
        {
            first_mouse_pos = Vector2f{ prev_mouse_pos };
        }
    }
    else {
        if (state == 1)
        {
            state = 2;
        }
    }

    Vector2f mouse_pos{ Mouse::getPosition(window) };

    switch (state)
    {
    case 1:
        // if more than 1 vertex exists, close track polygon if current mouse position is close to first set vertex
        if (track.getVertexCount() > 1 && v_magnitude(mouse_pos - first_mouse_pos) < 20.f)
        {
            // reset and update first track to set
            state = 0;
            if (finished_track_outline)
            {
                finished_both = true;
            }
            finished_track_outline = true;
            track.append(Vertex{ first_mouse_pos });
            first_mouse_pos = { -1, -1 };
        }
        else {
            track.append(Vertex{ mouse_pos });
            // Append another copy to be manipulated or to close polygon
            track.append(Vertex{ mouse_pos });
        }
        break;

    case 2:
        // If user moves mouse without click, adjust visually where next point will be set if clicked
        track[track.getVertexCount() - 1].position = mouse_pos;
        break;
    }
}

bool LevelEditor::done_with_edit() const
{
    return finished_both;
}

void LevelEditor::save_to_file(const char* file_name) const
{
    std::ofstream f_out{ file_name };
    if (!f_out)
    {
        std::cout << "file could not be opened!" << std::endl;
        return;
    }
    f_out << track_outline.getVertexCount() << std::endl;
    for (auto i = 0; i < track_outline.getVertexCount(); ++i)
    {
        const auto& p = track_outline[i].position;
        f_out << p.x << " " << p.y << std::endl;
    }
    f_out << track_inline.getVertexCount() << std::endl;
    for (auto i = 0; i < track_inline.getVertexCount(); ++i)
    {
        const auto& p = track_inline[i].position;
        f_out << p.x << " " << p.y << std::endl;
    }
}