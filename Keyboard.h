#pragma once

#include<queue>
#include<bitset>

class Keyboard {
	//so gfx can get the priv stuff
	friend class gfxWindow;
public:
	//event singleton
	class Event {
	public: enum class Type {
		Press, Release, Invalid
	};
	private:
		Type type;
		unsigned char code;
	public:
		Event() noexcept :
			type(Type::Invalid),
			code(0u)
		{}
		Event(Type type, unsigned char code) noexcept :
			type(type),
			code(code)
		{}
		bool IsPress() const noexcept {
			return type == Type::Press;
		}
		bool IsRelease()const noexcept {
			return type == Type::Release;
		}
		bool IsValid() const noexcept {
			return type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept {
			return code;
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	//key event stuff
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;
	//char event stuff
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	//autorepeat control
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutoRepeatIsEnabled() const  noexcept;
private:
	//functions used by window class
	//just register the messages
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autoRepeatEnabled = false;
	std::bitset<nKeys> keyStates;
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
};
