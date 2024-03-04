#include"Keyboard.h"




bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept {
	//keycode is just an index into the bitset set of 256 bools
	return keyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept {
	//check if there is stuff in the buffer first
	if (keyBuffer.size() > 0u) {
		/*
		Queue of events if there are events the event e = the first event in and this is 
		then popped as we no longer need it
		*/
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	else {
		Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const noexcept {
	return keyBuffer.empty();
}

char Keyboard::ReadChar() noexcept {
	if (charBuffer.size() > 0u) {
		unsigned char charCode = charBuffer.front();
		charBuffer.pop();
		return charCode;
	}
	else {
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const noexcept {
	return charBuffer.empty();
}

void Keyboard::FlushKey() noexcept {
	//Default constructs a new queue replaces old one
	//Essentialy deletes the queue
	keyBuffer = std::queue<Event>();
}

void Keyboard::FlushChar() noexcept {
	charBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept {
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept {
	autoRepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept {
	autoRepeatEnabled = false;
}


bool Keyboard::AutoRepeatIsEnabled() const noexcept {
	return autoRepeatEnabled;
}


void Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
	keyStates[keycode] = true;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
	keyStates[keycode] = false;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(char character)noexcept {
	charBuffer.push(character);
	TrimBuffer(charBuffer);
}

void Keyboard::ClearState() noexcept {
	keyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}