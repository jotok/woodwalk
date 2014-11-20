#pragma once

// #include "StateStack.hpp"
#include "Data.hpp"
#include "Resources.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>
#include <vector>

namespace sf {
    class RenderWindow;
}

class StateStack;

class State {
    public:
        enum class ID {
            None, Title, Game
        };

        typedef std::unique_ptr<State> Ptr;

        struct Context {
            Context(sf::RenderWindow &window, 
                    TextureHolder &textures, 
                    FontHolder &fonts,
                    ShaderHolder &shaders);
            sf::RenderWindow *window;
            TextureHolder *textures;
            FontHolder *fonts;
            ShaderHolder *shaders;
        };

    public:
        State(StateStack &stack, Context context);
        virtual ~State();
        
        virtual void draw() = 0;
        virtual bool update(sf::Time dt) = 0;
        virtual bool handleEvent(const sf::Event &event) = 0;

    protected:
        void requestStackPush(State::ID id);
        void requestStackPop();
        void requestStateClear();

        Context getContext() const;

    private:
        StateStack *_stack;
        Context _context;
};
