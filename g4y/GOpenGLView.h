#ifndef __GOPENGLVIEW_H__
#define __GOPENGLVIEW_H__
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include <SDL2/SDL.h>
#include "GConfig.hpp"

class GShader;
class G4Y_DLL GOpenGLView
{
public:
    int Init(const boost::property_tree::ptree& cfg);

	std::shared_ptr<GShader> GetShader();

	bool WindowShouldClose();

    int PollEvents();

    void BeginRender();

    void EndRender();

    void Exit();

private:
    std::shared_ptr<GShader> m_main_shader;
	bool            m_quit;
	SDL_Window*     m_wnd;
	SDL_GLContext   m_glContext;
};

#endif