#include <iostream>
#include <string>
#include <vector>

struct Date {
	int day;
	int month;
	int year;
};

enum type {
	PRODUCT,
	PERISHABLE_PRODUCT,
	ELECTRONIC_PRODUCT,
	BUILDING_MATERIAL
};



class Product {
   private:
	std::string name;
	int productID;
	int shelfLife;
	double weight;
   protected:
	double price;


   public:
	Product(const std::string& productName, int id, double productWeight, double productPrice, int shelf) {
		name = productName;
		productID = id;
		weight = productWeight;
		price = productPrice;
		shelfLife = shelf;
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
	virtual double calculateStorageWeight(double p) { return weight * price; };
	virtual void displayInfo() const {
		std::cout << "Name of product: " << name << "\n";
		std::cout << "ID: " << productID << "\n";
		std::cout << "Shelf life: " << shelfLife << "\n";
		std::cout << "Price: " << price << "\n";
		std::cout << "Weight: " << weight << std::endl;
	}
	virtual type getType() const {
		return PRODUCT;
	}
	std::string getName() const {
		return name;
	}
	int getID() const {
		return productID;
	}
	int getShelLife() const {
		return shelfLife;
	}
	double getWeight() const {
		return weight;
	}
	double getPrice() const {
		return price;
	}
};

class PerishableProduct : public Product {
   private:
	Date expirationDate;

   public:
	double calculateStorageWeight(double newPrice) override {
		price = newPrice;
		return newPrice * getWeight();
	}
	type getType() const override {
		return PERISHABLE_PRODUCT;
	}
};

class ElectronicProduct : Product {
   private:
	int warrantyPeriod;
	int powerRating;

   public:
	void displayInfo() const override {
		std::cout << "Name of product: " << getName() << "\n";
		std::cout << "ID: " << getID() << "\n";
		std::cout << "Shelf life: " << getShelLife() << "\n";
		std::cout << "Price: " << price << "\n";
		std::cout << "Weight: " << getWeight() << "\n";
		std::cout << "Warranty period: " << warrantyPeriod << "\n";
		std::cout << "Power rating: " << powerRating << std::endl;
	}
	type getType() const override {
		return ELECTRONIC_PRODUCT;
	}
};

class BuildingMaterial : Product {
   private:
	bool flammability;
   public:
	double calculateStorageWeight(double newPrice) override{
		price = newPrice;
		return newPrice * getWeight();
	}
	type getType() const override {
		return BUILDING_MATERIAL;
	}
};

class Warehouse {
   private:
	std::vector<Product*> products;
   public:
	std::vector<Product*> getExpiringProducts(int days) {}

	void displayInventory() const {
		for (Product* product: products) {
			std::cout << "--------------PERISHABLE PRODUCT--------------" << std::endl;
			if (product->getType() == PERISHABLE_PRODUCT) product->displayInfo();
		}
		for (Product* product: products) {
			std::cout << "--------------ELECTRONIC PRODUCT--------------" << std::endl;
			if (product->getType() == ELECTRONIC_PRODUCT) product->displayInfo();
		}
		for (Product* product: products) {
			std::cout << "--------------BUILDING MATERIAL----------------" << std::endl;
			if (product->getType() == BUILDING_MATERIAL) product->displayInfo();
		}
	}

};