#pragma once

#include "State.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <functional>
#include <map>
#include <vector>

class StateStack : private sf::NonCopyable {
    public:
        enum class Action {
            Push, Pop, Clear
        };

    public:
        explicit StateStack(State::Context context);

        template <typename T>
        void registerState(State::ID id);

        void update(sf::Time dt);
        void draw();
        void handleEvent(const sf::Event& event);

        void pushState(State::ID id);
        void popState();
        void clearStates();

        bool isEmpty() const;

    private:
        struct PendingChange {
            explicit PendingChange(Action action, State::ID stateID = State::ID::None);
            Action action;
            State::ID stateID;
        };

    private:
        State::Ptr createState(State::ID id);
        void applyPendingChanges();

    private:
        std::vector<State::Ptr> _stack;
        std::vector<PendingChange> _pendingList;
        State::Context _context;
        std::map<State::ID, std::function<State::Ptr()>> _factories;

};

template <typename T>
void StateStack::registerState(State::ID id) {
    _factories[id] = [this]() {
        return State::Ptr(new T(*this, _context));
    };
}
