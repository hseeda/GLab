// Dear ImGui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs
#include "pch.h"
#include "GLabSystem.h"
#include "WindowSystem.h"
#include "EventSystem.h"
#include "TextSystem.h"
#include "FileSystem.h"
#include "ShaderSystem.h"
#include "BodySystem.h"
#include "RenderSystem.h"

int main(int, char**)
{
	pwd();
    //_pl(openFileDialog());
    WS::init();
    //GS::init();
	SS::init();
    SS::print();
    TS::init();
    BS::pre_init();
    


    int win1 = WS::addImguiWindow("GLab Options");

    WS::addText(win1,"Camera Controls");
    WS::addSeparator(win1);
    BS::camera->addImguiControls(win1, "Orth Projection");

	auto ff = [](ES::event e) {if (e.data.b(0)) _pl(e.data.ID); };
	auto fslider = [](ES::event e) {_pl(e.data.ID, ": ", e.data.f(0)); };
	auto fcol = [](ES::event e) {_pl(e.data.ID, ": ", e.data.f(0), ",", e.data.f(1), ",", e.data.f(2)); };


    ES::subscribe(ES::imgui_slider, fslider, WS::addSlider(win1,"SL1",.5,0,1));
    WS::addCheckbox(win1, "TEST1", false);
    WS::addCheckbox(win1, "TEST2", false,true);
    WS::addCheckbox(win1, "TEST3", false);

    ES::subscribe(ES::imgui_checkbox, ff, WS::addCheckbox(win1, "TEST4", false));
    ES::subscribe(ES::imgui_button, ff, WS::addButton(win1, "Button   1"));

	//RS::FBRendererMS renderer;
 //   //renderer.setMultisample(4);
	////RS::Renderer renderer;
 //   renderer.init(16);
      
    auto bb = BS::addBody(1);
    bb->build();

    bb->addImguiControls(win1, "wireframe");
    

	//auto b = BS::addBody(1);

	//FS::readAbaqusInpFile(b->vertexData, b->indexData, "../Misc/models/aq123.inp");
	//for (int i = 0; i < b->vertexData.size(); i += 3)
	//{
	//	float x = b->vertexData[i];
	//	float y = b->vertexData[i + 1];
	//	float z = b->vertexData[i + 2];
	//	float n = glm::vec3(x, y, z).length();
	//	b->colorData.push_back(x / n);
	//	b->colorData.push_back(y / n);
	//	b->colorData.push_back(z / n);
	//}

    BS::init();

	//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
    // Main loop
    while (glfwGetKey(WS::window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(WS::window))
    {
        
            
        glfwPollEvents();
        ES::flush();     
        WS::preRenderOverlay();
        
        if (ES::isUpdateNeeded()) 
        {        
            
            //_p(".");
            WS::preRender();

            //GS::do1();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glLineWidth(5);
            glPointSize(5);
            
            BS::do1();
            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //b.do1(1);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);


            // Cull triangles which normal is not towards the camera
            //glEnable(GL_CULL_FACE);

            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //glDisable(GL_DEPTH_TEST);

            //if (/*WS::mouse_down == 1*/ 1) {
            //    for (int i = 0; i < 36 * 3; i += 3) {
            //        GLfloat x = GS::gg[i];
            //        GLfloat y = GS::gg[i + 1];
            //        GLfloat z = GS::gg[i + 2];

            //        glm::vec3 vertPos = glm::vec3(x, y, z);
            //        glm::vec2 pos;
            //        if (GS::cam->to2D(vertPos, pos))
            //            TS::RenderChar('O', pos.x, pos.y, 0.5f, glm::vec3(0.9, 0.1f, 0.2f));
            //    }
            //}
            //}
            TS::RenderText("GLAB Rev 0.1 (C)", 25.0f, 25.0f, 1.0f, glm::vec3(0.3, 0.3f, 0.3f));
            ES::updated();
		}
		WS::postRender();
        
        WS::postRenderOverlay();
        glfwSwapBuffers(WS::window);

    }
	//HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH   
    
    TS::exit();
    BS::exit();
    SS::exit();
    //GS::exit();
    WS::exit();



    return 0;
}
