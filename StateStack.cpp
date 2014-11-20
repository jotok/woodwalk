#include "StateStack.hpp"

#include <cassert>

StateStack::StateStack(State::Context context)
: _context{context}
{}

void StateStack::update(sf::Time dt) {
    // Iterate from top to bottom, stop as soon as update() returns false
    for (auto itr = _stack.rbegin(); itr != _stack.rend(); ++itr) {
        if (!(*itr)->update(dt))
            break;
    }

    applyPendingChanges();
}

void StateStack::draw() {
    // Draw all active states from bottom to top
    for (State::Ptr &state: _stack)
        state->draw();
}

void StateStack::handleEvent(const sf::Event &event) {
    // Iterate from top to bottom, stop as soon as handleEvent() returns false
    for (auto itr = _stack.rbegin(); itr != _stack.rend(); ++itr) {
        if (!(*itr)->handleEvent(event))
            break;
    }

    applyPendingChanges();
}

void StateStack::pushState(State::ID id) {
    _pendingList.push_back(PendingChange(Action::Push, id));
}

void StateStack::popState() {
    _pendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates() {
    _pendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const {
    return _stack.empty();
}

State::Ptr StateStack::createState(State::ID id) {
    auto found = _factories.find(id);
    assert(found != _factories.end());

    return found->second();
}

void StateStack::applyPendingChanges() {
    for (auto change: _pendingList) {
        switch (change.action) {
            case Action::Push:
                _stack.push_back(createState(change.stateID));
                break;

            case Action::Pop:
                _stack.pop_back();
                break;

            case Action::Clear:
                _stack.clear();
                break;
        }

        _pendingList.clear();
    }
}

StateStack::PendingChange::PendingChange(Action action, State::ID stateID) 
: action{action}
, stateID{stateID}
{}
