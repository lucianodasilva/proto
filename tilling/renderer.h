#pragma once

#ifndef _renderer_h_
#define _renderer_h_

class render_target;

class renderer {
private:



public:

    void set_target ( render_target * target );

    //render_target * get_target ();

    void clear ();

    void present ();

};


#endif