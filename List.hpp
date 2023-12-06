#include <iostream>
#include <exception>
#include <string>
//========================
// Описание исключений для обработчика исключений
//========================

namespace Errors {
	class ListException : public std::exception {
	public:
		virtual const char* what() const throw() {
			return "List Exception";
		}
	};

	class ListUnderflow : public ListException {
	public:
		const char* what() const throw() override {
			return "List is Empty";
		}
	};

	class ItemNF : public ListException {
	public:
		const char* what() const throw() override {
			return "Item not found";
		}
	};

	class IncorrectPType : public ListException {
	public:
		const char* what() const throw() override {
			return "Incorrect part type number";
		}
	};

	class IncorrectINum : public ListException {
	public:
		const char* what() const throw() override {
			return "Incorrect item number";
		}
	};

	class IncorrectData : public ListException {
	private:
		std::string errorMessage;
	public:
		IncorrectData(std::string& message) : errorMessage(message) {}

        const char* what() const throw() override {
			std::string error = "Incorrect input data for next field: " + this->errorMessage;
            return error.c_str();
		}
	};
}

//========================
// Объявление и описание абстрактных классов
//========================

class List;
class Item
{
	Item* prev;
	Item* next;
	List* list;
	friend class List;
public:
	Item() : prev(nullptr), next(nullptr), list(nullptr) {}
	virtual ~Item() { 
		if (list)
			removeItem();
	}

	Item* Next() const { return next; }
	Item* Prev() const { return prev; }
	List* getList() const { return list; }
	
	virtual unsigned int Id() const = 0;
	virtual std::string ProductCode() const = 0;
	virtual void delPart() = 0;
	virtual bool operator==( int type ) = 0;

	void removeItem();

	virtual void print(bool partsOnly, int& i) const = 0;
};

class List
{
protected:
	Item* head;
	Item* tail;
	int size;
public:
	List() : head(nullptr), tail(nullptr), size(0) {}
	virtual ~List() {}

	void setHead(Item* item) {
		head = item;
	}

	void setTail(Item* item) {
		tail = item;
	}

    Item* Head() const { return head; }
	Item* Tail() const { return tail; }
	int Size() const { return size; }
	void SizeDecrement() { size--; }

	virtual void delFById(unsigned int id) = 0;
	void appendToList(Item* item);
	void operator+=(Item* item) {
		appendToList(item);
	}
	Item* getItemByNum(const int num) const;
	void insertIntoList(Item* item, const int num);
	Item* removeItemByNum(const int num);
};