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
    virtual bool setData(SubjList& subjList);
    virtual bool setData() { return false; }

    Item* toItem() { return static_cast<Item*>(this); }
    bool operator<=( Part& other);
    bool operator==( int type ) override;

    unsigned int Id() const override { return id; }
    void delPart() override;

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
    ~SubjList() override {
        clear();
    }

    static void menu();

	void printList(bool partsOnly) const;
    void printList(unsigned int id) const;
    void printList(std::string productCode) const;
    Part* searchById(unsigned int id) const;
    int detailCount() const;

    Part& operator[](int index) const {
        return *(Part*)getItemByNum(index);
    }

    void delFById(unsigned int id) override;
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
    ~Detail() override {}

    bool setData() override;
    void getUserMaterial();
    void getUserDParams(bool isLength);
    void getUserProductCode();

    Part* toPart() { return (Part*)this; }
    std::string ProductCode() const override { return productCode; }
    double Length() const { return length; }
    double Width() const { return width; }
    
    void print() const override;
};

class Furniture : public Part
{
    unsigned int count;
public:
    Furniture() : Part(2) {}
    ~Furniture() override {}

    bool setData(SubjList& subjList) override;
    std::string ProductCode() const override { return 0; };
    
    unsigned int getUserId(SubjList& subjList);
    void getUserCount();

    void print() const override;
};