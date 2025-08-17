#include "pch.hpp"

#include "platform/xplatform/xcursor.hpp"
#include "platform/input.hpp"
#include "platform/window.hpp"
#include "platform/xplatform/xwindow.hpp"

#include "GLFW/glfw3.h"

BM_START

std::unordered_map<XCursor::Type, GLFWcursor*> XCursor::m_cursors{};
unsigned int XCursor::m_ref_count = 0;

void XCursor::setCursor(Type type)
{
    glfwSetCursor(Input::getCurrentWindow()->getNativeWindow(), m_cursors[type]);
}

void XCursor::init()
{
    if (++m_ref_count > 1) return;

    m_cursors =
    {
        { XCursor::Type::Arrow,      glfwCreateStandardCursor(GLFW_ARROW_CURSOR) },
        { XCursor::Type::IBeam,      glfwCreateStandardCursor(GLFW_IBEAM_CURSOR) },
        { XCursor::Type::Crosshair,  glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR) },
        { XCursor::Type::Hand,       glfwCreateStandardCursor(GLFW_HAND_CURSOR) },
        { XCursor::Type::VResize,    glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR) },
        { XCursor::Type::HResize,    glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR) },
        { XCursor::Type::DRLResize,  glfwCreateStandardCursor(GLFW_RESIZE_NESW_CURSOR) },
        { XCursor::Type::DLRResize,  glfwCreateStandardCursor(GLFW_RESIZE_NWSE_CURSOR) },
        { XCursor::Type::AResize,    glfwCreateStandardCursor(GLFW_RESIZE_ALL_CURSOR) },
        { XCursor::Type::NotAllowed, glfwCreateStandardCursor(GLFW_NOT_ALLOWED_CURSOR) }
    };

    BM_CORE_INFO("XCursor initialized");
}

void XCursor::destroy()
{
    if (--m_ref_count > 0) return;

    for (auto& [type, cursor] : m_cursors)
        if (cursor)
            glfwDestroyCursor(cursor);
    m_cursors.clear();
    BM_CORE_INFO("XCursor destroyed");
}

BM_END
