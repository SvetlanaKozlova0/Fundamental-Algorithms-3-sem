#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#define COUNT_SECONDS_DAY 86400
#define EPSILON 0.0000001

enum type { PRODUCT, PERISHABLE_PRODUCT, ELECTRONIC_PRODUCT, BUILDING_MATERIAL };

class Product {
   private:
	std::string name;
	int shelfLife;
	double weight;


   protected:
	double price;
	int productID;
	void mutatorPrice(double newPrice) { price = newPrice; }
	std::string getName() const { return name; }
	int getShelfLife() const { return shelfLife; }
	double getWeight() const { return weight; }

   public:
	virtual double getPrice() const { return price; }
	virtual std::time_t getTime() const { return {}; };
	virtual type getType() const { return PRODUCT; }
	virtual int getID() const { return productID; }

	Product(const std::string& productName, int id, double productWeight, double productPrice, int shelf) {
		name = productName;
		productID = id;
		weight = productWeight;
		price = productPrice;
		shelfLife = shelf;
	}
	Product() {
		name = "";
		productID = 0;
		weight = 0;
		price = 0;
		shelfLife = 0;
	}
	Product(const Product& p) {
		name = p.name;
		productID = p.productID;
		weight = p.weight;
		price = p.price;
		shelfLife = p.shelfLife;
	}
	Product& operator=(const Product& p) {
		name = p.name;
		productID = p.productID;
		weight = p.weight;
		price = p.price;
		shelfLife = p.shelfLife;
		return *this;
	}
	virtual ~Product() = default;
	virtual double calculateStorageFee(double) { return weight * price; }
	virtual void displayInfo() const {
		std::cout << "Name of product: " << getName() << "\n";
		std::cout << "ID: " << getID() << "\n";
		std::cout << "Shelf life: " << getShelfLife() << "\n";
		std::cout << "Price: " << getPrice() << "\n";
		std::cout << "Weight: " << getWeight() << "\n";
	}

};

class PerishableProduct : public Product {
   private:
	std::time_t expirationDate;
   protected:
	type getType() const override { return PERISHABLE_PRODUCT; }
	std::time_t getTime() const override { return expirationDate; }
	double getPrice() const override {return price;}
	int getID() const override {return productID;}
   public:

	PerishableProduct(const std::string& productName, int id, double productWeight, double productPrice, int shelf,
	                  int day, int month, int year)
	    : Product(productName, id, productWeight, productPrice, shelf) {
		std::time_t currentTime = std::time(nullptr);
		std::tm exp = {};
		if (year < 1900) year = 1900;
		exp.tm_year = year - 1900;
		if (month < 1) month = 1;
		exp.tm_mon = month - 1;
		if (day < 0 || day > 31) day = 1;
		exp.tm_mday = day;
		exp.tm_hour = 0;
		exp.tm_min = 0;
		exp.tm_sec = 0;
		exp.tm_isdst = -1;
		expirationDate = std::mktime(&exp);
		if (expirationDate > currentTime) {
			expirationDate = currentTime;
		}
	}
	PerishableProduct() : Product() { expirationDate = {}; }
	double calculateStorageFee(double newPrice) override {
		if (newPrice - getPrice() > EPSILON)
		mutatorPrice(newPrice);
		return getPrice() * getWeight();
	}

	void displayInfo() const override {
		std::cout << "Name of product: " << getName() << "\n";
		std::cout << "ID: " << getID() << "\n";
		std::cout << "Shelf life: " << getShelfLife() << "\n";
		std::cout << "Price: " << getPrice() << "\n";
		std::cout << "Weight: " << getWeight() << "\n";
		std::tm* exp = std::localtime(&expirationDate);
		std::cout << "Expiration date: ";
		if (exp) {
			std::cout << std::put_time(exp, "%d.%m.%Y") << std::endl;
		} else {
			std::cout << "Invalid date" << std::endl;
		}
		std::cout << "\n";
	}
};

class ElectronicProduct : public Product {
   private:
	int warrantyPeriod;
	int powerRating;

   protected:
	type getType() const override { return ELECTRONIC_PRODUCT; }
	std::time_t getTime() const override { return {}; }
	double getPrice() const override {return price;}
	int getID() const override {return productID;}
   public:

	ElectronicProduct(const std::string& productName, int id, double productWeight, double productPrice, int shelf,
	                  int warranty, int rating)
	    : Product(productName, id, productWeight, productPrice, shelf) {
		if (warranty < 0) warranty = 0;
		warrantyPeriod = warranty;
		if (rating < 0) rating = 0;
		powerRating = rating;
	}
	ElectronicProduct() : Product() {
		warrantyPeriod = 0;
		powerRating = 0;
	}
	void displayInfo() const override {
		std::cout << "Name of product: " << getName() << "\n";
		std::cout << "ID: " << getID() << "\n";
		std::cout << "Shelf life: " << getShelfLife() << "\n";
		std::cout << "Price: " << getPrice() << "\n";
		std::cout << "Weight: " << getWeight() << "\n";
		std::cout << "Warranty period: " << warrantyPeriod << "\n";
		std::cout << "Power rating: " << powerRating << "\n" << std::endl;
	}

};

class BuildingMaterial : public Product {
   private:
	bool flammability;
   protected:
	type getType() const override { return BUILDING_MATERIAL; }
	std::time_t getTime() const override { return  {}; }
	double getPrice() const override {return price;}
	int getID() const override {return productID;}
   public:

	BuildingMaterial(const std::string& productName, int id, double productWeight, double productPrice, int shelf,
	                 bool flam)
	    : Product(productName, id, productWeight, productPrice, shelf) {
		flammability = flam;
	}
	double calculateStorageFee(double newPrice) override {
		if (!flammability) return getPrice() * getWeight();
		if (newPrice - price > EPSILON)
		mutatorPrice(newPrice);
		return getPrice() * getWeight();
	}
	void displayInfo() const override {
		std::cout << "Name of product: " << getName() << "\n";
		std::cout << "ID: " << getID() << "\n";
		std::cout << "Shelf life: " << getShelfLife() << "\n";
		std::cout << "Price: " << getPrice() << "\n";
		std::cout << "Weight: " << getWeight() << "\n";
		std::cout << "Flammability: ";
		if (flammability)
			std::cout << "yes\n";
		else
			std::cout << "no\n";
		std::cout << std::endl;
	}
};

class Warehouse {
   private:
	std::vector<Product*> products;
   public:
	std::vector<Product*> getExpiringProducts(int days) const {
		std::vector<Product*> expired;
		std::time_t currentTime = std::time(nullptr);
		std::time_t endTime = currentTime + (days * COUNT_SECONDS_DAY);
		for (Product* product : products) {
			if (product->getType() == PERISHABLE_PRODUCT) {
				if (product->getTime() <= endTime) {
					expired.push_back(product);
				}
			}
		}
		return expired;
	}

	void displayInventory() const {
		std::cout << "----------------PERISHABLE PRODUCT----------------" << std::endl;
		for (Product* product : products) {
			if (product->getType() == PERISHABLE_PRODUCT) {
				product->displayInfo();
			}
		}
		std::cout << "----------------ELECTRONIC PRODUCT----------------" << std::endl;
		for (Product* product : products) {
			if (product->getType() == ELECTRONIC_PRODUCT) {
				product->displayInfo();
			}
		}
		std::cout << "----------------BUILDING MATERIAL----------------" << std::endl;
		for (Product* product : products) {
			if (product->getType() == BUILDING_MATERIAL) {
				product->displayInfo();
			}
		}
		std::cout << "-------------PRODUCT WITHOUT CATEGORY-------------" << std::endl;
		for (Product* product : products) {
			if (product->getType() == PRODUCT) {
				product->displayInfo();
			}
		}
	}
	Warehouse& operator+=(Product* current) {
		for (Product* product : products) {
			if (product->getID() == current->getID()) {
				std::cout << "An error: this ID is already exist.\n\n";
				return *this;
			}
		}
		products.push_back(current);
		return *this;
	}
	Warehouse& operator-=(int id) {
		std::vector<Product*>:: iterator iter = std::remove_if(products.begin(), products.end(), [id](Product* product) {
			return product->getID() == id;
		});
		if (iter != products.end()) {
			products.erase(iter, products.end());
		}
		return *this;
	}

	Product* operator[](int id) {
		for (Product* product : products) {
			if (product->getID() == id) {
				return product;
			}
		}
		return nullptr;
	}

	size_t getAmountProducts() const { return products.size(); }

	double getTotalCost() const {
		double cost = 0;
		for (Product* product : products) {
			cost += product->calculateStorageFee(product->getPrice());
		}
		return cost;
	}
	void printByCategory(type t) const {
		for (Product* product : products) {
			if (product->getType() == t) product->displayInfo();
		}
	}
};

std::ostream& operator<<(std::ostream& out, const Warehouse& current) {
	out << "--------------------WAREHOUSE---------------------" << "\n";
	out << "Amount products: " << current.getAmountProducts() << "\n";
	out << "--------------------------------------------------" << "\n";
	current.displayInventory();
	out << "\n";
	return out;
}

int main() {
	std::cout << "Create a perishable product: " << std::endl;
	PerishableProduct milk("milk", 1, 1.0, 120, 21, 24, 11, 2024);
	milk.displayInfo();
	std::cout << "Let's change price: (to 200)" << std::endl;
	std::cout << milk.calculateStorageFee(200);
	milk.displayInfo();
	std::cout << "Let's change the value to a smaller one (it is incorrect): " << std::endl;
	milk.calculateStorageFee(100);
	milk.displayInfo();
	std::cout << "Nothing is changed.\n" << std::endl;
	std::cout << "Create an electronic product: " << std::endl;
	ElectronicProduct tv("tv", 2, 1.7, 1200, 100, 100, 10);
	tv.displayInfo();
	std::cout << "It's impossible to change price: " << std::endl;
	tv.calculateStorageFee(100);
	tv.displayInfo();
	std::cout << "Let's create a building material: " << std::endl;
	BuildingMaterial paper("paper", 5, 1.1, 400, 100, true);
	paper.displayInfo();
	std::cout << "Let's change price to (800): " << std::endl;
	paper.calculateStorageFee(800);
	paper.displayInfo();
	std::cout << "It's impossible to make smaller price: " << std::endl;
	paper.calculateStorageFee(300);
	paper.displayInfo();


	std::cout << "Let's make another building material (without flammability): " << std::endl;
	BuildingMaterial clay("clay", 3, 1.5, 560, 300, false);
	clay.displayInfo();
	std::cout << "Let's change price (to 1000): (it is impossible because this material has not flammability)" << std::endl;
	clay.calculateStorageFee(1000);
	clay.displayInfo();
	std::cout << "Nothing changed\n" << std::endl;

	std::cout << "Let's make a base product: " << std::endl;
	Product chair("chair", 4, 1.4, 100,100);
	chair.displayInfo();
	std::cout << "\nLet's change price of chair:  (it is impossible for ordinary products)" << std::endl;
	chair.calculateStorageFee(300);
	std::cout << "\nAfter changing to 300: (nothing changed)" << std::endl;
	chair.displayInfo();
	std::cout << "\nLet's make a copy of chair (operator =) " << std::endl;
 	Product newChair = chair;
	newChair.displayInfo();
	std::cout << std::endl;

	std::cout << "Let's make a warehouse: " << std::endl;
	Warehouse current;
	std::cout << "\nWarehouse before adding anything: " << std::endl;
	std::cout << current;
	std::cout << "\nLet's add milk: " << std::endl;
	current += &milk;
	std::cout << current;
	std::cout << "Let's add tv, paper, chair, clay:" << std::endl;
	current += &tv;
	current += &paper;
	current += &chair;
	current += &clay;
	std::cout << current;
	std::cout << "Let's create a product with same id and try to add: " << std::endl;
	PerishableProduct bread("bread", 2, 100, 67, 2, 23, 11, 2023);
	current += &bread;
	std::cout << current;
	std::cout << "Nothing was added.\n" << std::endl;
	std::cout << "Let's delete chair: (id 4)" << std::endl;
	current -= 4;
	std::cout << current;
	std::cout << "Let's find clay: (id 3)" << std::endl;
	current[3]->displayInfo();
	std::cout << "\nLet's get total cost: " << current.getTotalCost() << std::endl;
	std::cout << "\nLet's add new perishable product: " << std::endl;
	PerishableProduct coffee("coffee", 12, 120, 1200, 400, 12 ,12 , 2025);
	current += &coffee;
	coffee.displayInfo();
	std::cout << "\nLet's find products that will be soon expired (10 days): " << std::endl;
	std::vector<Product*> expired = current.getExpiringProducts(10);
	for (Product* p : expired) p->displayInfo();
	std::cout << "Let's change price of milk: " << std::endl;
	milk.calculateStorageFee(900);
	std::cout << "Milk now: ";
	current[1]->displayInfo();
	std::cout << "Let's watch all products by categories:" << std::endl;
	current.displayInventory();
	std::cout << "\n" << std::endl;
	std::cout << "Let's find building materials: " << std::endl;
	current.printByCategory(BUILDING_MATERIAL);
	return 0;
}