#include <GL/glew.h>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

int main(int arg_c, char * arg_v[]) {

    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 32;

    sf::RenderWindow window (sf::VideoMode (800, 600), "Tilling", sf::Style::Default, contextSettings);
    window.setVerticalSyncEnabled(false);

    window.setActive(true);

    GLenum err = glewInit ();
    if (GLEW_OK != err) {
        window.close ();
        return EXIT_FAILURE;
    }

    while (window.isOpen ()) {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close ();

            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
                window.close ();

            //if (event.type == sf::Event::Resized)
                //glViewport //

            // render stuffs
            window.display ();
        }
    }

    return EXIT_SUCCESS;
}