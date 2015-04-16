#pragma once
#ifndef _proto_renderer_h_
#define _proto_renderer_h_

namespace proto {

    class render_target;

    class renderer {
    private:

    public:

        void set_target ( render_target * target );

        //render_target * get_target ();

        void clear ();

        void present ();

    };

}

#endif