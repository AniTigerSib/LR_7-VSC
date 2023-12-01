#include "Subj.hpp"
// for numeric_limits
#include <limits>

//========================
// Реализация static методов
//========================

void SubjList::menu() {
    CommandLineUI::printMainMenu();
    int k = 0;
    int i = 0;
    int intInput = 0;
    SubjList* subjList = new SubjList();
    Part* tempPart = nullptr;
    Detail* tDetail = nullptr;
    Furniture* tFurniture = nullptr;
    std::string productCode = "";
    while (true) {
        input<int>(k);
        switch (k) {
        case 1:
            std::cout << "Choose type of part you want to create:" << std::endl;
            i = 1;
            for (const auto& t : partTypes) {
                std::cout << i << ".\t" << t.second << std::endl;
                i++;
            }
            input<int>(intInput);
            switch (intInput) {
            case 1:
                tDetail = new Detail();
                try {
                    if (tDetail->Part::setData(*subjList)) {
                        //*subjList += (tDetail->toItem()); // TODO: Рассмотреть
                        subjList->appendToList(tDetail->toItem());
                    }
                    else {
                        delete tDetail;
                        tDetail = nullptr;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Caught an exception: " << e.what() << std::endl;
                    delete tDetail;
                    tDetail = nullptr;
                }
                break;
            case 2:
                tFurniture = new Furniture();
                try {
                    if (tFurniture->Part::setData(*subjList)) {
                        //*subjList += (tFurniture->toItem());
                        subjList->appendToList(tFurniture->toItem());
                    }
                    else {
                        delete tFurniture;
                        tFurniture = nullptr;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Caught an exception: " << e.what() << std::endl;
                    delete tFurniture;
                    tFurniture = nullptr;
                }
                break;
            default:
                std::cout << "Incorrect input part type. Try again." << std::endl;
                break;
            }
            break;
        case 2:
            try {
                subjList->printList(false);
            } catch (const std::exception& e) {
                std::cerr << "Caught an exception: " << e.what() << std::endl;
            }
            break;
        case 3:
            if (subjList->Size() == 0)
                std::cout << "List is empty" << std::endl;
            else
                std::cout << "Items in the list: " << subjList->Size() << std::endl;
            break;
        case 4:
            std::cout << "Product code:" << std::endl;
            input<std::string>(productCode);
            try {
                subjList->printList(productCode);
            } catch (const std::exception& e) {
                std::cerr << "Caught an exception: " << e.what() << std::endl;
            }
            break;
        case 5:
            subjList->sort();
            break;
        case 6:
            subjList->clear();
            break;
        case 7:
            std::cout << "Position of item, you want to delete:" << std::endl;
            input<int>(k);
            try {
                tempPart = (Part*)subjList->getItemByNum(k);
                tempPart->delPart(*subjList);
                break;
            } catch (const std::exception& e) {
                std::cerr << "Caught an exception: " << e.what() << std::endl;
            }
        case 8:
            std::cout << "\033[2J" << std::endl; // аналог system("cls") для linux
            CommandLineUI::printMainMenu();
            break;
        case 9:
            delete subjList;
            subjList = nullptr;
            return;
        default:
            std::cout << "Incorrect input number. Try again." << std::endl;
            break;
        }
    }
}

//========================
// Реализация вспомогательных функций работы с потоком ввода
//========================

void resetInputState() {
    std::cin.clear(); // сброс состояния потока ввода при ошибке 
}
void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // очистка буфера ввода
}
template <typename T>
inline void input(T &input) {
	while (true) {
		std::cout << "> ";
		if (std::cin >> input){
			clearInput();
			return;
		}
		else {
			std::cout << "Incorrect input data type. Needed " << typeid(input).name() << " type. Try again." << std::endl;
			resetInputState();
			clearInput();
		}
	}
}

//========================
// Реализация функций пространства имён CommandLineUI
//========================

void CommandLineUI::printMainMenu() {
    std::cout << "Welcome! Choose what do you want to do (enter number in console):" << std::endl;
	for (const auto& name : CommandLineUI::mainMenuTexts)
		std::cout << name << std::endl;
}

//========================
// Реализация функций класса Part
//========================

Part::Part(int typeNum) : type(typeNum) {
    if (typeNum == 1) {
        static unsigned int tempID = 0;
        id = tempID;
	    tempID++;
    }
}

void Part::delPart(SubjList& subjList) {
    Furniture* tFurniture = nullptr;
    Detail* tDetail = nullptr;
    unsigned int id = 0;
    switch (this->type)
    {
    case 1:
        id = this->id;
        subjList.delFById(id);
        tDetail = static_cast<Detail*>(this);
        delete tDetail;
        break;
    case 2:
        tFurniture = static_cast<Furniture*>(this);
        delete tFurniture;
        break;
    default:
        throw Errors::IncorrectPType();
        break;
    }
    return;
}

bool Part::setData(SubjList& subjList) {
    const auto& t = partTypes.at(this->type);
    std::cout << "You need to set data for this " << t << std::endl;
    std::cout << "Name of " << t << ":" << std::endl;
    input<std::string>(this->name);
    switch(type){
    case 1:
        if (!(this)->setData())
            return false;
        break;
    case 2:
        if (!(this)->setData(subjList))
            return false;
        break;
    default:
        throw Errors::IncorrectPType();
    }
    return true;
}

void Part::print() const {
    std::cout << "Name:\t" << this->name << std::endl;
    std::cout << "\tId:\t" << this->id << std::endl;
    const auto& t = partTypes.at(this->type);
    std::cout << "\tType (part):\t" << t << std::endl;
}

void Part::print(bool partsOnly, int& i) const {
    switch (type) {
    case 1:
        std::cout << "[" << i << "]\t";
        this->Part::print();
        if (!partsOnly) this->print();
        i++;
        break;
    case 2:
        if (!partsOnly) {
            std::cout << "[" << i << "]\t";
            this->Part::print();
            this->print();
        }
        i++;
        break;
    default:
        throw Errors::IncorrectPType();
        break;
    }
}

//========================
// Реализация функций класса SubjList
//========================

void SubjList::printList(bool partsOnly) const {
    if (!this->Size())
        throw Errors::ListUnderflow();
    int i = 0;
    Item* temp = this->Head();
    std::cout << "Subjects:" << std::endl;
    std::cout << std::endl;
    while (temp) {
        temp->print(partsOnly, i);
        std::cout << std::endl;
        temp = temp->Next();
    }
}

void SubjList::printList(unsigned int id) const {
    if (!this->Size())
        throw Errors::ListUnderflow();
    int i = 0;
    Item* temp = this->Head();
    std::cout << "Subjects:" << std::endl;
    std::cout << std::endl;
    while (temp) {
        if (temp->Id() == id){
            temp->print(false, i);
            std::cout << std::endl;
        }
        temp = temp->Next();
    }
}

void SubjList::printList(std::string productCode) const {
    if (!this->Size())
        throw Errors::ListUnderflow();
    Item* temp = this->Head();
    while (temp) {
        if (temp->Type() == 1 && ((Detail*)temp)->ProductCode() == productCode){
            this->printList(temp->Id());
            std::cout << std::endl;
            return;
        }
        temp = temp->Next();
    }
    std::cout << "Items not found by this product code" << std::endl;
    return;
}

Part* SubjList::searchById(unsigned int id) const {
    if (!this->Size())
        throw Errors::ListUnderflow();
    Item* temp = this->Head();
    while (temp) {
        if (temp->Type() == 1 && temp->Id() == id)
            return (Part*)temp;
        temp = temp->Next();
    }
    return nullptr;
}

void SubjList::delFById(unsigned int id) {
    if (!this->Size())
        return;
    Item* temp = this->Head();
    Item* tempNext = nullptr;
    while (temp) {
        tempNext = temp->Next();
        if (temp->Type() == 2 && temp->Id() == id) {
            ((Part*)temp)->delPart(*this);
        }
        temp = tempNext;
    }
    return;
}

int SubjList::detailCount() const {
    int count = 0;
    Item* temp = this->Head();
    while (temp) {
        if (temp->Type() == 1)
            count++;
        temp = temp->Next();
    }
    return count;
}

bool SubjList::sortCompare(Item* i1, Item* i2) {
    Part* p1 = static_cast<Part*>(i1);
    Part* p2 = static_cast<Part*>(i2);
    if (p1->type == 1 && p2->type == 1) {
        Detail* d1 = static_cast<Detail*>(p1);
        Detail* d2 = static_cast<Detail*>(p2);
        double dOneSqare = d1->Length() * d1->Width();
        double dTwoSqare = d2->Length() * d2->Width();
        if (dOneSqare < dTwoSqare) {
            return true;
        }
    }
    if (p1->type == 2 && p2->type == 1)
        return true;
    return false;
}

void SubjList::sort() {
    if (!this->Size())
        throw Errors::ListUnderflow();
    Item* item = nullptr;
    for (int i = 0; i < this->Size() - 1; i++) {
        for (int j = 0; j < this->Size() - i - 1; j++) {
            if (this->sortCompare(this->getItemByNum(j), this->getItemByNum(j + 1))) {
                item = this->removeItemByNum(j + 1);
                this->insertIntoList(item, j);
            }
        }
    }
    item = nullptr;
    int dc = this->detailCount();
    if (dc == this->Size())
        return;
    for (int i = 0; i < dc; i++) {
        Detail* detail = static_cast<Detail*>(this->getItemByNum(i));
        unsigned int id = detail->id;
        for (int j = dc; j < this->Size(); j++) {
            Part* part = static_cast<Part*>(this->getItemByNum(j));
            if (part->id == id) {
                item = this->removeItemByNum(j);
                i++;
                dc++;
                if (i == this->Size())
                    this->appendToList(item);
                else
                    this->insertIntoList(item, i);
            }
        }
    }
    return;
}

void SubjList::clear() {
	if (this->Size() == 0) {
		std::cout << "List is already empty" << std::endl;
		return;
	}
	Part* tempPart = (Part*)getItemByNum(0);
	while(true){
		tempPart->delPart(*this);
        try {
            tempPart = (Part*)getItemByNum(0);
        } catch (const std::exception& e) {
            if (e.what() == Errors::ListUnderflow().what()) {
                std::cout << "List was cleared" << std::endl;
	            return;
            }
            else {
                std::cerr << "Caught an exception: " << e.what() << std::endl;
            }
        }
	}
}

//========================
// Реализация функций класса Detail
//========================

bool Detail::setData() {
    while (true) {
        try {
            this->getUserMaterial();
            this->getUserDParams(true);
            this->getUserDParams(false);
            this->getUserProductCode();
            return true;
        } catch (std::exception& e) {
            std::cerr << "Caught an exception: " << e.what() << std::endl;
            std::cout << "Do you want to try again?\n(Y/N)"<< std::endl;
            char choice;
            input<char>(choice);
            if (choice == 'Y' || choice == 'y')
                continue;
            else
                return false;
        }
    }
}

void Detail::getUserMaterial() {
    int tMaterial = 0;
    std::cout << "Types of detail material:\n";
    int i = 1;
    for (const auto& t : materialTypes) {
        std::cout << i << ".\t" << t.second << std::endl;
        i++;
    }
    std::cout << "Num of material type:" << std::endl;
    input<int>(tMaterial);
    auto it = materialTypes.find(tMaterial);
    if (it == materialTypes.end())
        throw Errors::IncorrectPType();
    this->material = it->first;
    return;
}

void Detail::getUserDParams(bool isLenght) {
    std::cout << "Enter the detail ";
    if (isLenght) {
        std::string length = "length";
        std::cout << length << ":" << std::endl;
        input<double>(this->length);
        if (this->length <= 0) {
            throw Errors::IncorrectData(length);
        }
    }
    else {
        std::string width = "width";
        std::cout << width << ":" << std::endl;
        input<double>(this->width);
        if (this->width <= 0) {
            throw Errors::IncorrectData(width);
        }
    }
    return;
}

void Detail::getUserProductCode() {
    std::cout << "Product code:" << std::endl;
    input<std::string>(this->productCode);
}

void Detail::print() const {
    const auto& t = materialTypes.at(this->material);
    std::cout << "\tType (material):\t" << t << std::endl;
    std::cout << "\tLenght:\t" << this->length << std::endl;
    std::cout << "\tWidth:\t" << this->width << std::endl;
    std::cout << "\tProduct code:\t" << this->productCode << std::endl;
}

//========================
// Реализация функций класса Furniture
//========================

bool Furniture::setData(SubjList& subjList) {
    while (true) {
        try {
            this->setId(this->getUserId(subjList));
            this->getUserCount();
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Caught an exception: " << e.what() << std::endl;
            std::cout << "Do you want to try again?\n(Y/N)"<< std::endl;
            char choice;
            input<char>(choice);
            if (choice == 'Y' || choice == 'y')
                continue;
            else
                return false;
        }
    }
}

unsigned int Furniture::getUserId(SubjList& subjList) {
    subjList.printList(true); 
    std::cout << "Enter detail ID for this furniture set:" << std::endl;
    int tId = 0;
    input<int>(tId);
    if (subjList.searchById(tId))
        return tId;
    else
        throw Errors::ItemNF();
}

void Furniture::getUserCount() {
    std::cout << "Enter the number of furniture sets:" << std::endl;
    input<unsigned int>(this->count);
    return;
}

void Furniture::print() const {
    std::cout << "\tCount:\t" << this->count << std::endl;
}