//
// @description: Create Windows demo
// @author: von.wu
// @date: 2021-06-26 16:14:36
//

#ifndef DRAW_CREATE_WINDOW_H
#define DRAW_CREATE_WINDOW_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "base_draw.h"

    // your code
    class DrawCreateWindow : public BaseDraw
    {
    private:
        /* data */
    public:
        DrawCreateWindow() : BaseDraw(){};
        ~DrawCreateWindow();
    };

    DrawCreateWindow::~DrawCreateWindow()
    {
    }
#ifdef __cplusplus
}
#endif
#endif // DRAW_CREATE_WINDOW_H