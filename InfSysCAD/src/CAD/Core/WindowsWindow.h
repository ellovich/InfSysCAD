#pragma once
#include <Aspect_DisplayConnection.hxx>
#include <Aspect_RenderingContext.hxx>
#include <Aspect_Window.hxx>
#include <Graphic3d_Vec.hxx>
#include <TCollection_AsciiString.hxx>

struct GLFWwindow;

namespace InfSysCAD
{
	class WindowsWindow : public Aspect_Window
	{
		DEFINE_STANDARD_RTTI_INLINE(WindowsWindow, Aspect_Window)
	public:
		WindowsWindow(const char* title, int w, int h);
		virtual ~WindowsWindow() { Close(); }

		void Close();

		int GetWidth() const { return m_xRight - m_xLeft; }
		int GetHeight() const { return m_yBottom - m_yTop; }

	public:
		const Handle(Aspect_DisplayConnection)& GetDisplay() const { return myDisplay; }
		GLFWwindow* GetGLFWwindow() { return m_GLFWwindow; }
		Aspect_RenderingContext NativeGlContext() const;
		Graphic3d_Vec2i CursorPosition() const;

		virtual Aspect_Drawable NativeHandle() const override;
		virtual Aspect_Drawable NativeParentHandle() const override { return 0; }
		virtual Aspect_TypeOfResize DoResize() override;
		virtual Aspect_FBConfig NativeFBConfig() const override { return nullptr; }
		virtual bool IsMapped() const override;
		virtual bool DoMapping() const override { return true; }
		virtual void Map() const override;
		virtual void Unmap() const override;
		virtual void Position(int& xLeft, int& yTop, int& xRight, int& yBottom) const override
		{
			xLeft = m_xLeft;
			xRight = m_xRight;
			yTop = m_yTop;
			yBottom = m_yBottom;
		}
		virtual double Ratio() const override { return double(m_xRight - m_xLeft) / double(m_yBottom - m_yTop); }
		virtual void Size(int& width, int& height) const override { width = GetWidth(); height = GetHeight(); }

	protected:
		GLFWwindow* m_GLFWwindow;
		Handle(Aspect_DisplayConnection) m_Display;
		int m_xLeft, m_yTop, m_xRight, m_yBottom;
	};
}