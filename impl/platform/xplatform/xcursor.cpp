module;

#include "GLFW/glfw3.h"

module bm.cursor;

import bm.window;
import bm.log;

import bm.input;

namespace bm 
{

std::unordered_map<Cursor::Type, GLFWcursor*> Cursor::m_cursors{};
unsigned int Cursor::m_ref_count = 0;

void Cursor::setCursor(Type type)
{
    glfwSetCursor(Input::getCurrentWindow()->getNativeWindow(), m_cursors[type]);
}

void Cursor::init()
{
    if (++m_ref_count > 1) return;

    m_cursors =
    {
        { Cursor::Type::Arrow,      glfwCreateStandardCursor(GLFW_ARROW_CURSOR) },
        { Cursor::Type::IBeam,      glfwCreateStandardCursor(GLFW_IBEAM_CURSOR) },
        { Cursor::Type::Crosshair,  glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR) },
        { Cursor::Type::Hand,       glfwCreateStandardCursor(GLFW_HAND_CURSOR) },
        { Cursor::Type::VResize,    glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR) },
        { Cursor::Type::HResize,    glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR) },
        { Cursor::Type::DRLResize,  glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR) },
        { Cursor::Type::DLRResize,  glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR) },
        { Cursor::Type::AResize,    glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR) },
        { Cursor::Type::NotAllowed, glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR) }
    };

    log::core::info("Cursor initialized");
}

void Cursor::destroy()
{
    if (--m_ref_count > 0) return;

    for (auto& [type, cursor] : m_cursors)
        if (cursor)
            glfwDestroyCursor(cursor);
    m_cursors.clear();
    log::core::info("Cursor destroyed");
}

}
