#include "EventHandler.hpp"

EventHandler::EventHandler(): callback(nullptr), sender(nullptr)
{
}

void EventHandler::Subscribe(const event_callback func, void* sender)
{
	this->callback = func;
	this->sender   = sender;
}


void EventHandler::Unsubscribe()
{
	this->callback = nullptr;
	this->sender   = nullptr;
}


void EventHandler::Invoke(void* args) const
{
	if (callback != nullptr)
	{
		callback(sender, args);
	}
}
