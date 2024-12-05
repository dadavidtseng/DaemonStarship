#pragma once

typedef void (*event_callback)(void* sender, void* args);

class EventHandler
{
public:
	EventHandler();

	void Subscribe(event_callback func, void* sender);

	void Unsubscribe();

	void Invoke(void* args) const;

private:
	event_callback callback;
	void*          sender;
};

struct EventArgs
{
	int value;
};
