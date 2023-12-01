#include "List.hpp"
#include <vector>
#include <map>

//========================
// Описание вспомогательного класса консоли
//========================

class ConsoleHelper {
public:
    ConsoleHelper() {
        setlocale(LC_ALL, "Russian");
    }
    ~ConsoleHelper() {
        std::cout << "Process exited. For exiting the console - press enter." << std::endl;
		std::cin.get();
    }
};

//========================
// Объявление вспомогательных функций работы с потоком ввода
//========================

void resetInputState();
void clearInput();
template <typename T>
void input(T &input);

//========================
// Объявление и описание пространства имён CommandLineUI
//========================

namespace CommandLineUI {
	const std::vector<std::string> mainMenuTexts = {
		"[1] Add to the list",
		"[2] Display the current list",
        "[3] Display the number of items in the list",
        "[4] Search for all parts and accessories by product code",
		"[5] Sort parts",
        "[6] Clear list",
		"[7] Delete element",
        "[8] Clear console",
		"[9] Exit"
	};
	
	void printMainMenu();
};

//========================
// Объявление и описание вспомогательных типов данных
//========================

/**
 * @brief Map for types of parts
 * @param int Key
 * @param string Type of part
 */
const std::map<int, std::string> partTypes = {
    {1, "Detail"},
    {2, "Furniture set"}
};

/**
 * @brief Map for types of materials
 * @param int Key
 * @param string Type of material
 */
const std::map<int, std::string> materialTypes = {
    {1, "Wood"},
    {2, "Glass"},
    {3, "Metal"}
};

//========================
// Объявление и описание базовых классов
//========================

class SubjList;
class Detail;
class Furniture;
class Part : public Item
{
    unsigned int id;
    std::string name;
    int type;
    friend class SubjList;
protected:
    Part(int typeNum);
    void setId(unsigned int id) { this->id = id; }
public:
    ~Part() {}

    virtual bool setData(SubjList& subjList);
    virtual bool setData() { return false; }

    Item* toItem() { return static_cast<Item*>(this); }

    int Type() const override { return type; }
    unsigned int Id() const override { return id; }
    void delPart(SubjList& subjList);

    virtual void print() const;
    void print(bool partsOnly, int& i) const override;
};

/**
 * @brief Производный класс уровня базовых классов от List. Имеет собственные методы
 * 
 */
class SubjList : public List
{
public:
    SubjList() {}
    ~SubjList() {
        clear();
    }

    static void menu();
	void printList(bool partsOnly) const;
    void printList(unsigned int id) const;
    void printList(std::string productCode) const;
    Part* searchById(unsigned int id) const;
    void delFById(unsigned int id);
    int detailCount() const;
    bool sortCompare(Item* i1, Item* i2) override;
    void sort();
    void clear();
};

//========================
// Объявление и описание специфических классов
//========================

class Detail : public Part
{
    int material;
    double length;
    double width;
    std::string productCode;
public:
    Detail() : Part(1) {}
    ~Detail() {}

    bool setData(SubjList& subjList) override { return false; }
    bool setData() override;
    void getUserMaterial();
    void getUserDParams(bool isLength);
    void getUserProductCode();

    Part* toPart() { return (Part*)this; }
    std::string ProductCode() const { return productCode; }
    double Length() const { return length; }
    double Width() const { return width; }
    
    void print() const override;
};

class Furniture : public Part
{
    unsigned int count;
public:
    Furniture() : Part(2) {}
    ~Furniture() {}

    bool setData(SubjList& subjList) override;
    bool setData() override { return false; }
    
    unsigned int getUserId(SubjList& subjList);
    void getUserCount();

    void print() const override;
};