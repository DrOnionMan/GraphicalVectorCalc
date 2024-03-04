#include<windows.h>
#include"Mouse.h"

//ill let you guess
std::pair<int, int> Mouse::GetPos() const noexcept {
	return { x,y };
}

//return x and y pos haway man
int Mouse::GetPosX() const noexcept {
	return x;
}

int Mouse::GetPosY() const noexcept {
	return y;
}

//again really Kid??
bool Mouse::LeftIsPressed() const noexcept {
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept {
	return rightIsPressed;
}

bool Mouse::MiddleIsPressed() const noexcept {
	return middleIsPressed;
}

bool Mouse::IsInWindow() const noexcept {
	return isInWindow;
}

/*
Here just same as in KBD Reads the event at the front of the queue into an event instance e, queue event is then popped
if no event is present rfeturns the default for event see Mouse.h
*/
Mouse::Event Mouse::Read() noexcept {
	if (buffer.size() > 0) {
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else {
		return Mouse::Event();
	}
}


//Just Clears the queue
void Mouse::Flush() noexcept {
	buffer = std::queue<Event>();
}

//sets Mouse x and y to new coords based on mouse move then pushes the move event onto the queue
void Mouse::OnMouseMove(int newx, int newy) noexcept {
	x = newx;
	y = newy;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

//same as above exept it changes leftIsPressed to true and pushes a LPress event onto queue
void Mouse::OnLeftPressed(int x, int y) noexcept {
	leftIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

//opposite of above
void Mouse::OnLeftReleased(int x, int y) noexcept {
	leftIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept {
	isInWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept {
	isInWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept {
	rightIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept {
	rightIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept {
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept {
	wheelDeltaCarry += delta;
	//generate events for every 120
	while (wheelDeltaCarry >= WHEEL_DELTA) {
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA) {
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}

}

void Mouse::OnMButtonDown(int x, int y) noexcept {
	middleIsPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::MPress, *this));
	TrimBuffer();
}

void Mouse::OnMButtonReleased(int x, int y) noexcept {
	middleIsPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::MRelease, *this));
	TrimBuffer();
}

//i wonder what this might do
void Mouse::TrimBuffer() noexcept {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}