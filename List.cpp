#include "List.hpp"

//========================
// Реализация функций класса Item
//========================

void Item::removeItem() {
	if (this->prev)
		prev->next = this->next;
	else
		list->setHead(this->next);

	if (this->next)
		next->prev = this->prev;
	else
		list->setTail(this->prev);
	this->list->SizeDecrement();
	this->list = nullptr;
	return;
}

//========================
// Реализация функций класса List
//========================

void List::appendToList(Item* item) {
	if (!size) {
		item->prev = nullptr;
		this->head = item;
	}
	else {
		item->prev = tail;
		this->tail->next = item;
	}
	this->tail = item;
	item->next = nullptr;
	item->list = this;
	size++;
	return;
}

Item* List::getItemByNum(const int num) {
    if (!size)
		throw Errors::ListUnderflow();
	if (num < 0)
		throw Errors::IncorrectINum();
	int i = 0;
	Item* tempItem = head;
	for (i = 0; tempItem && i < num; i++)
		tempItem = tempItem->next;
	if (!tempItem)
		throw Errors::ItemNF();
	return tempItem;
}

void List::insertIntoList(Item* item, const int num) {
    try {
		Item* tempItem = getItemByNum(num);
		item->prev = tempItem->prev;
		item->next = tempItem;
		if (tempItem->prev)
			tempItem->prev->next = item;
		tempItem->prev = item;
		if (tempItem == head)
			head = item;
		item->list = this;
		size++;
		return;
	} catch (std::exception& e) {
		std::string msg = e.what();
		if (!msg.compare("List is Empty") and num == 0)
			appendToList(item);
		else
			std::cerr << "Caught an exception: " << e.what() << std::endl;
	}
	return;
}

Item* List::removeItemByNum(const int num) {
	try {
		Item* item = getItemByNum(num);
		item->removeItem();
		return item;
	} catch (std::exception& e) {
		std::cerr << "Caught an exception: " << e.what() << std::endl;
		return nullptr;
	}
}