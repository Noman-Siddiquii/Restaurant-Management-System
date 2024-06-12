#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <climits>
#include <cfloat>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <future>
#include <sstream>
#include <algorithm>
#include <limits>
using namespace std;

class Payment {
		string cvv;
		string cardHolderName;
		string accNum;
	public:
		string getCvv() {
			return cvv;
		}
		string getName() {
			return cardHolderName;
		}
		string getAccNum() {
			return accNum;
		}
		void Pay(string t, int amount) {
			if (t == "Cash") {

			}
			if (t == "Online") {
				
				while (true) {
					cout << "Enter Account Number:";
					cin >> accNum;
					if (accNum.length() != 16 || !all_of(accNum.begin(), accNum.end(), ::isdigit)) {
						cout << "Invalid Account Number. Please enter a 16-digit numeric value." << endl;
					} else {
						break;
					}
				}


				while (true) {
					cout << "Enter CVV:";
					cin >> cvv;
					if (cvv.length() != 3 || !all_of(cvv.begin(), cvv.end(), ::isdigit)) {
						cout << "Invalid CVV. Please enter a 3-digit numeric value." << endl;
					} else {
						break;
					}
				}


				while (true) {
					cout << "Enter Card Holder Name:";
					cin.ignore();
					getline(cin, cardHolderName);
					if (cardHolderName.empty() || !all_of(cardHolderName.begin(), cardHolderName.end(), ::isalpha)) {
						cout << "Invalid Card Holder Name. Please enter a valid name containing only alphabets." << endl;
					} else {
						break;
					}
				}

				cout << "Payment of PKR " << amount << " is successfully paid from account number ************"
				     << accNum[12] << accNum[13] << accNum[14] << accNum[15] << endl;
			}
		}
};

class Sales {
	public:
		static double sale;
		Sales() {
			sale=0;
		}

		void AddSales(int amount) {
			sale=sale+amount;
		}

		void DisplaySales() {
			cout <<"Total Sales for the day: " << sale << endl;
		}
};

double Sales::sale=0;


class MenuItem {
	public:
		string itemName;
		double itemPrice;

		MenuItem(string name, double price) : itemName(name), itemPrice(price) {}
};

class DeliveryArea {
	public:
		string areaName;
		vector<pair<string, double>> neighbors;

		DeliveryArea(string name) : areaName(name) {}

		void addNeighbor(string neighbor, double time) {
			neighbors.push_back(make_pair(neighbor, time));
		}
};

class Menu {
	public:
		struct Node {
			static int id;
			int index;
			MenuItem data;
			Node* next;
			Node(MenuItem item) : data(item), next(nullptr) {
				id++;
				index=id;
			}
		};


		Node* head;

		Menu() : head(nullptr) {}
		string getIndexItem(int index);
		void addItem(MenuItem item) {
			Node* newNode = new Node(item);

			if (head == nullptr) {
				head = newNode;
			} else {
				Node* temp = head;
				while (temp->next != nullptr) {
					temp = temp->next;
				}

				temp->next = newNode;
			}
		}

		void removeItem(int index) {
			if (head == nullptr) {
				cout << "Menu is empty. Cannot remove item." << endl;
				return;
			}

			Node* current = head;
			Node* prev = nullptr;

			while (current != nullptr && current->index != index) {
				prev = current;
				current = current->next;
			}

			if (current == nullptr) {
				cout << "Invalid index. Item not found." << endl;
				return;
			}

			if (prev == nullptr) {
				head = current->next;
			} else {
				prev->next = current->next;
			}

			delete current;
			cout << "Item at index " << index << " removed successfully." << endl;
		}

		void displayMenu() {
			Node* current = head;
			int i=0;
			while (current != nullptr) {
				cout <<"["<<++i<<"]"<< current->data.itemName << " - PKR " << current->data.itemPrice << endl;
				current = current->next;
			}
		}

		MenuItem* searchItem(string itemName) {
			Node* current = head;
			while (current != nullptr) {
				if (current->data.itemName == itemName) {
					return &current->data;
				}
				current = current->next;
			}
			return nullptr;
		}
};

int Menu::Node::id = 0;

struct OrderNode {
	int orderNumber;
	int minTime;
	int height;
	pair<string, int>* items;
	int itemCount;
	double totalAmount;
	string deliveryArea;
	bool isDineIn;
	OrderNode* left;
	OrderNode* right;

	OrderNode(int orderNo, bool dineIn, string area)
		: orderNumber(orderNo), totalAmount(0.0), height(0), isDineIn(dineIn), deliveryArea(area), left(nullptr), right(nullptr), items(nullptr), itemCount(0), minTime(0) {}

	void addItem(const string& itemName, int quantity) {
		if (items == nullptr) {
			items = new pair<string, int>[1];
		} else {
			pair<string, int>* newItems = new pair<string, int>[itemCount + 1];
			for (int i = 0; i < itemCount; ++i) {
				newItems[i] = items[i];
			}
			delete[] items;
			items = newItems;
		}

		items[itemCount++] = make_pair(itemName, quantity);
	}
};

class AVLTree {
	public:
		static OrderNode* root;

		static int height(OrderNode* node);
		static int getBalance(OrderNode* node);
		static OrderNode* rotateRight(OrderNode* y);
		static OrderNode* rotateLeft(OrderNode* x);
		static OrderNode* insert(OrderNode* node, OrderNode* order);
		static OrderNode* minValueNode(OrderNode* node);
		static OrderNode* remove(OrderNode* node, int orderNo);
		static void displayOrdersPreparing(OrderNode* node);
		static OrderNode* search(OrderNode* node, int orderNo);
};

OrderNode* AVLTree::root = nullptr;

int AVLTree::height(OrderNode* node) {
	if (node == nullptr)
		return 0;
	return node->height;
}

int AVLTree::getBalance(OrderNode* node) {
	if (node == nullptr)
		return 0;
	return height(node->left) - height(node->right);
}

OrderNode* AVLTree::rotateRight(OrderNode* y) {
	OrderNode* x = y->left;
	OrderNode* T2 = x->right;

	x->right = y;
	y->left = T2;

	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

	return x;
}

OrderNode* AVLTree::rotateLeft(OrderNode* x) {
	OrderNode* y = x->right;
	OrderNode* T2 = y->left;

	y->left = x;
	x->right = T2;

	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;

	return y;
}

OrderNode* AVLTree::insert(OrderNode* node, OrderNode* order) {
	if (node == nullptr)
		return order;

	if (order->orderNumber < node->orderNumber)
		node->left = insert(node->left, order);
	else if (order->orderNumber > node->orderNumber)
		node->right = insert(node->right, order);

	node->height = 1 + max(height(node->left), height(node->right));

	int balance = getBalance(node);
	if (balance > 1 && order->orderNumber < node->left->orderNumber)
		return rotateRight(node);

	if (balance < -1 && order->orderNumber > node->right->orderNumber)
		return rotateLeft(node);

	if (balance > 1 && order->orderNumber > node->left->orderNumber) {
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}

	if (balance < -1 && order->orderNumber < node->right->orderNumber) {
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}

	return node;
}

OrderNode* AVLTree::minValueNode(OrderNode* node) {
	OrderNode* current = node;

	while (current->left != nullptr)
		current = current->left;

	return current;
}

OrderNode* AVLTree::remove(OrderNode* node, int orderNo) {
	if (node == nullptr)
		return node;

	if (orderNo < node->orderNumber)
		node->left = remove(node->left, orderNo);
	else if (orderNo > node->orderNumber)
		node->right = remove(node->right, orderNo);
	else {
		if ((node->left == nullptr) || (node->right == nullptr)) {
			OrderNode* temp = (node->left != nullptr) ? node->left : node->right;

			if (temp == nullptr) {
				temp = node;
				node = nullptr;
			} else
				*node = *temp;
			delete temp;
		} else {
			OrderNode* temp = minValueNode(node->right);

			node->orderNumber = temp->orderNumber;

			node->right = remove(node->right, temp->orderNumber);

			delete[] temp->items;
			delete temp;
		}
	}

	if (node == nullptr)
		return node;

	node->height = 1 + max(height(node->left), height(node->right));

	int balance = getBalance(node);

	if (balance > 1 && getBalance(node->left) >= 0)
		return rotateRight(node);

	if (balance > 1 && getBalance(node->left) < 0) {
		node->left = rotateLeft(node->left);
		return rotateRight(node);
	}

	if (balance < -1 && getBalance(node->right) <= 0)
		return rotateLeft(node);

	if (balance < -1 && getBalance(node->right) > 0) {
		node->right = rotateRight(node->right);
		return rotateLeft(node);
	}

	return node;
}
OrderNode* AVLTree::search(OrderNode* node, int orderNo) {
	if (node == nullptr || node->orderNumber == orderNo)
		return node;

	if (orderNo < node->orderNumber)
		return search(node->left, orderNo);
	else
		return search(node->right, orderNo);
}


void AVLTree::displayOrdersPreparing(OrderNode* node) {
	if (node == nullptr)
		return;

	displayOrdersPreparing(node->left);
	cout << "Order #" << node->orderNumber << ":";
	for (int i = 0; i < node->itemCount; ++i)
		cout << " " << node->items[i].second << "x " << node->items[i].first;
	cout << " - PKR " << node->totalAmount << " (" << (node->isDineIn ? "Dine-In" : "Delivery to " + node->deliveryArea) << ")" << endl;
	displayOrdersPreparing(node->right);
}




class Rider {
	public:
		string riderName;
		string contact;
		bool available;
		chrono::system_clock::time_point availableTime; 

		Rider(string name) : riderName(name), available(true) {}

		void setUnavailableForDuration(int durationSeconds) {
			available = false;  
			availableTime = chrono::system_clock::now() + chrono::seconds(durationSeconds);
		}

};



class Manager {
	public:
		static int orderNumber;
		Sales s;
		vector<Rider> riders;

		void placeOrder(Menu& menu, const vector<DeliveryArea>& deliveryAreas);
		void prepared(int orderNo);
		void displayOrdersPreparing();
		void assignRider();
		Rider newRider(const string& name, const string& contact);
		void addRider(const string& name, const string& contact);
		void removeRider(const string& name);
		void displayRiders() const;
		void assignRider(double minTime);
		void displayAvailableRiders() const;
};

int Manager::orderNumber = 1;

void Manager::assignRider(double minTime) {

	minTime=minTime*2;
	if (!riders.empty()) {
		int availableRidersCount = 0;


		for (Rider& rider : riders) {
			if (rider.available) {
				++availableRidersCount;
			}
		}

		if (availableRidersCount > 0) {
			srand(time(0));


			int randomIndex = rand() % riders.size();
			Rider& selectedRider = riders[randomIndex];
			while(!selectedRider.available) {
				randomIndex = rand() % riders.size();
				selectedRider = riders[randomIndex];
			}

			selectedRider.setUnavailableForDuration(static_cast<int>(minTime));

			cout << "Order assigned to Rider: " << selectedRider.riderName << endl;
			cout << "Rider " << selectedRider.riderName << " is on the way."<<endl;
			cout<<"Rider's contact no.: "<<selectedRider.contact<<endl;
		} else {
			cout << "No available riders." << endl;
		}
	} else {
		cout << "No riders available." << endl;
	}

}



void Manager::displayRiders() const {
	cout << "Riders List:" << endl;
	for (const Rider& rider : riders) {
		cout << "Name: " << rider.riderName << ", Contact: " << rider.contact<<endl;
	}
}

void Manager::displayAvailableRiders() const {
	cout << "Riders List:" << endl;
	for (const Rider& rider : riders) {
		if(rider.available) {
			cout << "Name: " << rider.riderName << ", Contact: " << rider.contact<<endl;
		}

	}
}

Rider Manager::newRider(const string& name, const string& contact) {
	Rider newRiderInstance(name);
	newRiderInstance.contact = contact;
	return newRiderInstance;
}

void Manager::addRider(const string& name, const string& contact) {
	Rider newRiderInstance = newRider(name, contact);
	riders.push_back(newRiderInstance);
}

void Manager::removeRider(const string& name) {
	for (vector<Rider>::iterator it = riders.begin(); it != riders.end(); ++it) {
		if (it->riderName == name) {
			it = riders.erase(it); 
			cout << "Rider removed successfully." << endl;
			return;
		}
	}

	cout << "Rider not found." << endl;
}

double findMinTime(const vector<DeliveryArea>& deliveryAreas, const string& start, const string& end) {
	int startIndex = -1, endIndex = -1;

	if (start == end) {
		return 5.0; 
	}

	for (int i = 0; i < deliveryAreas.size(); ++i) {
		if (deliveryAreas[i].areaName == start) {
			startIndex = i;
		} else if (deliveryAreas[i].areaName == end) {
			endIndex = i;
		}

		if (startIndex != -1 && endIndex != -1) {
			break;
		}
	}

	if (startIndex == -1 || endIndex == -1) {
		cout << "Invalid start or end area." << endl;
		return -1.0;
	}

	int numAreas = deliveryAreas.size();
	vector<double> dist(numAreas, DBL_MAX);
	vector<bool> visited(numAreas, false);

	dist[startIndex] = 0;

	for (int count = 0; count < numAreas - 1; ++count) {
		int minDistIndex = -1;
		double minDist = DBL_MAX;

		for (int i = 0; i < numAreas; ++i) {
			if (!visited[i] && dist[i] < minDist) {
				minDist = dist[i];
				minDistIndex = i;
			}
		}

		if (minDistIndex == -1) {
			break;
		}

		visited[minDistIndex] = true;

		for (const auto& neighbor : deliveryAreas[minDistIndex].neighbors) {
			int neighborIndex = -1;
			for (int i = 0; i < numAreas; ++i) {
				if (deliveryAreas[i].areaName == neighbor.first) {
					neighborIndex = i;
					break;
				}
			}

			if (neighborIndex != -1 && !visited[neighborIndex] && dist[minDistIndex] != DBL_MAX &&
			        dist[minDistIndex] + neighbor.second < dist[neighborIndex]) {
				dist[neighborIndex] = dist[minDistIndex] + neighbor.second;
			}
		}
	}

	return dist[endIndex];
}



void Manager::placeOrder(Menu& menu, const vector<DeliveryArea>& deliveryAreas) {
	cout << "Menu:\n";
	menu.displayMenu();

	vector<pair<string, int>> orderItems;
	string orderItem;
	int quantity, choice;

	do {
		while (true) {
			cout << "Enter the item you want to order (type 0 to finish): ";
			if (cin >> choice && (choice == 0 || (choice >= 1 && choice <= Menu::Node::id))) {
				break;
			} else {
				cin.clear();  
				cin.ignore(numeric_limits<streamsize>::max(), '\n');  
				cout << "Invalid input. Please enter a valid numeric value for choice.\n";
			}
		}

		if (choice == 0) {
			break;
		}

		orderItem = menu.getIndexItem(choice);
		if (orderItem == "Invalid Index") {
			cout << "Please choose correctly!\n";
			continue;
		}

		while (true) {
			cout << "Enter the quantity for " << orderItem << ": ";
			if (cin >> quantity && quantity > 0 ) {
				if(quantity>100)
				{
					cout<<"max quantity for "<<orderItem<<" is 100"<<endl;
				}
				break;
		
			} else {
				cin.clear();  
				cin.ignore(numeric_limits<streamsize>::max(), '\n');  
				cout << "Invalid input. Please enter a valid numeric value for quantity.\n";
			}
		}

		orderItems.push_back(make_pair(orderItem, quantity));
	} while (true);

	double totalAmount = 0.0;
	string deliveryArea;
	for (const auto& item : orderItems) {
		MenuItem* selectedItem = menu.searchItem(item.first);
		if (selectedItem != nullptr) {
			totalAmount += selectedItem->itemPrice * item.second;
		}
	}

	int orderType;
	cout << "Is the order for dine-in or delivery? (1 for delivery, 2 for dine-in): ";


	while (true) {
		if (cin >> orderType && (orderType == 1 || orderType == 2)) {
			break;  
		} else {
			cin.clear();  
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
			cout << "Invalid input. Please enter 1 for delivery or 2 for dine-in.\n";
		}
	}

	bool isDineIn = (orderType != 1);

	if (isDineIn) {
		deliveryArea = "";
	} else {
		cout << "Available delivery areas:\n";
		int i = 0;
		for (const auto& area : deliveryAreas) {
			cout << "- [" << ++i << "] " << area.areaName << endl;
		}
		string addr;

		while (true) {
			cout << "Select delivery area (1 to " << deliveryAreas.size() << "): ";
			if (cin >> choice && (choice >= 1 && choice <= deliveryAreas.size())) {
				cout<<"Enter complete address: ";
				cin.ignore();
				getline(cin, addr);
				break;
			} else {
				cin.clear();  
				cin.ignore(numeric_limits<streamsize>::max(), '\n');  
				cout << "Invalid input. Please enter a valid numeric value for delivery area.\n";
			}
		}

		deliveryArea = deliveryAreas[--choice].areaName;
	}
	cout<<"Order ID: "<<orderNumber<<endl;
	cout << "Total Amount: PKR " << totalAmount << endl;
	s.AddSales(totalAmount);

	string confirm;
	cout << "Confirm order? (y/n): ";
	cin >> confirm;

	while (confirm != "y" && confirm != "Y" && confirm != "n" && confirm != "N") {
		cout << "Invalid choice!" << endl;
		cout << "Confirm order? (y/n): ";
		cin >> confirm;
	}

	if (confirm == "y" || confirm == "Y") {
		OrderNode* order = new OrderNode(orderNumber, isDineIn, deliveryArea);

		if (!isDineIn) {
			double minTime = findMinTime(deliveryAreas, "Muhammad Ali", deliveryArea);

			if (minTime >= 0) {
				order->minTime = minTime;
			}
		}

		for (const auto& item : orderItems) {
			order->addItem(item.first, item.second);
		}
		order->totalAmount = totalAmount;
		AVLTree::root = AVLTree::insert(AVLTree::root, order);
		orderNumber++;
		system("cls");
	}
	Payment payment;
	int ch = 0;
	bool validInput = false;

	if (!isDineIn) {
		while (!validInput) {
			cout << "How would you like to pay\n";
			cout << "[1] Cash on delivery\n[2] Online Payment\n";
			cout << "Enter choice:";

		
			if (cin >> ch) {
				switch (ch) {
					case 1:
						payment.Pay("Cash", totalAmount);
						validInput = true;
						break;
					case 2:
						payment.Pay("Online", totalAmount);
						validInput = true;
						break;
					default:
						cout << "Wrong Input. Please enter 1 or 2.\n";
	
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
			} else {
				cout << "Invalid Input. Please enter a valid integer.\n";

				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		}
	}


	std::this_thread::sleep_for(std::chrono::seconds(5));
	system("cls");
}
string Menu::getIndexItem(int index) {
	Node* current = head;
	while (current != nullptr) {
		if (current->index == index) {
			return current->data.itemName;
		}
		current = current->next;
	}
	return "Invalid Index";
}


void Manager::prepared(int orderNo) {
	OrderNode* order = AVLTree::search(AVLTree::root, orderNo);


	if (order == nullptr) {
		cout << "Order not found." << endl;
		return;
	}



	if (order->isDineIn) {
		cout << "Dine-in order #" << orderNo << " served." << endl;
	} else {
		cout << "Delivery order #" << orderNo << " prepared." << endl;
		cout<<"Will be delivered in "<<order->minTime<<" minutes"<<endl;
		assignRider(order->minTime);
	}

	AVLTree::root = AVLTree::remove(AVLTree::root, orderNo);
}

void Manager::displayOrdersPreparing() {
	cout << "\nOrders in Preparing:\n";
	AVLTree::displayOrdersPreparing(AVLTree::root);
}

void getCurrentDateAndDay(int& year, int& month, int& day, std::string& weekday) {

	std::time_t currentTime = std::time(nullptr);


	std::tm* localTime = std::localtime(&currentTime);

	year = localTime->tm_year + 1900; 
	month = localTime->tm_mon + 1;    
	day = localTime->tm_mday;         


	const char* weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	weekday = weekdays[localTime->tm_wday]; // Day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
}


int main() {
	std::cout << "			 __          __  _                            _______    \n"
	          "			 \\ \\        / / | |                          |__   __|   \n"
	          "			  \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___     | | ___  \n"
	          "			   \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\    | |/ _ \\ \n"
	          "			    \\  /\\  /  __/ | (_| (_) | | | | | |  __/    | | (_) |\n"
	          "			     \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|    |_|\\___/ \n";


	std::cout << "  _____           _                              _     __  __                                                   _   \n"
	          " |  __ \\         | |                            | |   |  \\/  |                                                 | |  \n"
	          " | |__) |___  ___| |_ __ _ _   _ _ __ __ _ _ __ | |_  | \\  / | __ _ _ __   __ _  __ _  ___ _ __ ___   ___ _ __ | |_ \n"
	          " |  _  // _ \\/ __| __/ _` | | | | '__/ _` | '_ \\| __| | |\\/| |/ _` | '_ \\ / _` |/ _` |/ _ \\ '_ ` _ \\ / _ \\ '_ \\| __|\n"
	          " | | \\ \\  __/\\__ \\ || (_| | |_| | | | (_| | | | | |_  | |  | | (_| | | | | (_| | (_| |  __/ | | | | |  __/ | | | |_ \n"
	          " |_|  \\_\\___||___/\\__\\__,_|\\__,_|_|  \\__,_|_| |_|\\__| |_|  |_|\\__,_|_| |_|\\__,_|\\__, |\\___|_| |_| |_|\\___|_| |_|\\__|\n"
	          "                                                                                 __/ |                              \n"
	          "                                                                                |___/                               \n";

	std::cout << "					  _____           _                 \n"
	          "					 / ____|         | |                \n"
	          "					| (___  _   _ ___| |_ ___ _ __ ___  \n"
	          "					 \\___ \\| | | / __| __/ _ \\ '_ ` _ \\ \n"
	          "					 ____) | |_| \\__ \\ ||  __/ | | | | |\n"
	          "					|_____/ \\__, |___/\\__\\___|_| |_| |_|\n"
	          "					        __/ |                       \n"
	          "					       |___/                        \n";

	sleep(3);
	system("cls");
	std::cout << "                      _____                 _                      _   _                                                   \n"
	          "                     |  __ \\               | |                    | | | |                                                  \n"
	          "                     | |  | | _____   _____| | ___  _ __   ___  __| | | |__  _   _                                         \n"
	          "                     | |  | |/ _ \\ \\ / / _ \\ |/ _ \\| '_ \\ / _ \\/ _` | | '_ \\| | | |                                        \n"
	          "                     | |__| |  __/\\ V /  __/ | (_) | |_) |  __/ (_| | | |_) | |_| |                                        \n"
	          "                     |_____/ \\___| \\_/ \\___|_|\\___/| .__/ \\___|\\__,_| |_.__/ \\__, |                                        \n"
	          "                                                   | |                        __/ |                                        \n"
	          "                     _       _  __                 |_|         ___  _____  __|___/    _  _   _____   __ _  _ __             \n"
	          "     /\\             | |     | |/ /                            / / |/ /__ \\|__ \\      | || | | ____| / /| || |\\ \\            \n"
	          "    /  \\   _ __  ___| |__   | ' /_   _ _ __ ___   __ _ _ __  | || ' /   ) |  ) |_____| || |_| |__  / /_| || |_| |           \n"
	          "   / /\\ \\ | '_ \\/ __| '_ \\  |  <| | | | '_ ` _ \\ / _` | '__| | ||  <   / /  / /______|__   _|___ \\| '_ \\__   _| |           \n"
	          "  / ____ \\| | | \\__ \\ | | | | . \\ |_| | | | | | | (_| | |    | || . \\ / /_ / /_         | |  ___) | (_) | | | | |           \n"
	          " /_/    \\_\\_| |_|___/_| |_| |_|\_\\__,_|_| |_| |_|\\__,_|_|    | ||_|\\_\\____|____|        |_| |____/ \\___/  |_| |_|           \n"
	          "  _   _                                        _              \\_\\       _    ___  _____  ___        _  _ ___ /_/___ _____  \n"
	          " | \\ | |                                 /\\   | |                      | |  / / |/ /__ \\|__ \\      | || |__ \\| ____|__ \\ \\ \n"
	          " |  \\| | ___  _ __ ___   __ _ _ __      /  \\  | |__  _ __ ___   ___  __| | | || ' /   ) |  ) |_____| || |_ ) | |__    ) | |\n"
	          " | . ` |/ _ \\| '_ ` _ \\ / _` | '_ \\    / /\\ \\ | '_ \\| '_ ` _ \\ / _ \\/ _` | | ||  <   / /  / /______|__   _/ /|___ \\  / /| |\n"
	          " | |\\  | (_) | | | | | | (_| | | | |  / ____ \\| | | | | | | | |  __/ (_| | | || . \\ / /_ / /_         | |/ /_ ___) |/ /_| |\n"
	          " |_| \\_|\\___/|_| |_| |_|\\__,_|_| |_| /_/    \\_\\_| |_|_| |_| |_|\\___|\\__,_| | ||_|\_\\____|____|        |_|____|____/|____| |\n\n";

	std::cout << "    /\\    / _|/ _|                 | |              / / |/ /__ \\|__ \\      | || | | || |____  | ____\\ \\                   " << std::endl;
	std::cout << "   /  \\  | |_| |_ __ _ _ __        | | __ _ _ __   | || ' /   ) |  ) |_____| || |_| || |_  / /| |__  | |                  " << std::endl;
	std::cout << "  / /\\ \\ |  _|  _/ _` | '_ \\   _   | |/ _` | '_ \\  | ||  <   / /  / /______|__   _|__   _|/ / |___ \\ | |                  " << std::endl;
	std::cout << " / ____ \\| | | || (_| | | | | | |__| | (_| | | | | | || . \\ / /_ / /_         | |    | | / /   ___) || |                  " << std::endl;
	std::cout << "/_/    \\_\\_| |_|\\__,_|_| |_|  \\____/ \\__,_|_| |_| | ||_|\\_\\____|____|        |_|    |_|/_/   |____/ |_|                  " << std::endl;
	std::cout << "                                                     \\_\\                                             /_/                   " << std::endl;
	sleep(3);
	system("cls");

	int year, month, day;
	std::string weekday;


	getCurrentDateAndDay(year, month, day, weekday);


	cout << "Current Date: " << year << '-' << month << '-' << day << std::endl;
	cout << "Current Day: " << weekday << std::endl;
	sleep(2);


	Sales salesInstance;

	Menu menu;
	menu.addItem(MenuItem("Coffee", 500));
	menu.addItem(MenuItem("Apple Juice", 200));
	menu.addItem(MenuItem("Orange Juice", 200));
	menu.addItem(MenuItem("Burger", 500));
	menu.addItem(MenuItem("Pizza", 1200));
	menu.addItem(MenuItem("French Fries", 200));
	menu.addItem(MenuItem("Tacos", 400));
	menu.addItem(MenuItem("Sandwhich", 400));
	menu.addItem(MenuItem("Chiceken Tikka", 450));
	menu.addItem(MenuItem("Biryani", 300));

	vector<DeliveryArea> deliveryAreas = {
		DeliveryArea("Muhammad Ali"),
		DeliveryArea("Bahadarabad"),
		DeliveryArea("Saddar"),
		DeliveryArea("PECHS"),
		DeliveryArea("Nursery"),
		DeliveryArea("Malir"),
		DeliveryArea("Korangi"),
		DeliveryArea("KDA"),
		DeliveryArea("DHA Phase 8")
	};


	deliveryAreas[0].addNeighbor("Bahadarabad", 10.0);
	deliveryAreas[0].addNeighbor("Saddar", 12.0);

	deliveryAreas[1].addNeighbor("Muhammad Ali", 10.0);
	deliveryAreas[1].addNeighbor("Saddar", 20.0);

	deliveryAreas[2].addNeighbor("Muhammad Ali", 12.0);
	deliveryAreas[2].addNeighbor("Bahadarabad", 20.0);
	deliveryAreas[2].addNeighbor("PECHS", 15.0);

	deliveryAreas[3].addNeighbor("Saddar", 15.0);
	deliveryAreas[3].addNeighbor("Muhammad Ali", 20.0);
	deliveryAreas[3].addNeighbor("Nursery", 7.0);

	deliveryAreas[4].addNeighbor("Malir", 10.0);
	deliveryAreas[4].addNeighbor("Korangi", 10.0);

	deliveryAreas[5].addNeighbor("Korangi", 3.0);
	deliveryAreas[5].addNeighbor("Muhammad Ali", 25.0);
	deliveryAreas[5].addNeighbor("DHA Phase 8", 8.0);

	deliveryAreas[6].addNeighbor("Malir", 3.0);
	deliveryAreas[6].addNeighbor("Muhammad Ali", 20.0);
	deliveryAreas[6].addNeighbor("KDA", 4.0);

	deliveryAreas[7].addNeighbor("Malir", 6.0);
	deliveryAreas[7].addNeighbor("DHA Phase 8", 8.0);

	deliveryAreas[8].addNeighbor("KDA", 8.0);
	deliveryAreas[8].addNeighbor("Malir", 8.0);



	Manager manager;
	manager.addRider("Shahan", "03323065674");
	manager.addRider("Ahan",   "03324556732");
	manager.addRider("Talha", " 03335067674");
	string rName;
	string rNum;
	int price;
	int index;
	string name;
	int choice;
	system("cls");
	do {
		cout << "\nEmployee Menu:\n";
		cout << "1. Display Menu\n";
		cout << "2. Place Order\n";
		cout << "3. Mark Order as Prepared\n";
		cout << "4. Display Orders in Preparing\n";
		cout << "5. Add Menu Item\n";
		cout << "6. Remove Menu Item\n";
		cout << "7. Add Rider\n";
		cout << "8. Remove Rider\n";
		cout << "9. Display All Riders\n";
		cout << "10. Display Available Riders\n";
		cout << "11. Display Sales\n";
		cout << "12. Exit / End of the day\n";
		cout << "Enter your choice: ";
		cin >> choice;
		int count=0;
		switch (choice) {
			case 1:
				cout << "\nMenu:\n";
				menu.displayMenu();
				break;
			case 2:
				manager.placeOrder(menu, deliveryAreas);
				break;
			case 3:
				manager.displayOrdersPreparing();
				int orderNo;

				
				while (true) {
					cout << "\nEnter the order number to mark as prepared: ";
					if (cin >> orderNo) {
						break;  
					} else {
						cin.clear(); 
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
						cout << "Invalid input. Please enter a valid numeric value for the order number.\n";
					}
				}

				count = 0;
				for (int i = 0; i < manager.riders.size(); i++) {
					if (!manager.riders[i].available) {
						count++;
					}
				}

				if (count == manager.riders.size()) {
					cout << "No riders available\nYour order will be dispatched soon\n";
					break;
				}

				manager.prepared(orderNo);
				std::this_thread::sleep_for(std::chrono::seconds(5));
				system("cls");
				break;
			case 4:
				manager.displayOrdersPreparing();
				std::this_thread::sleep_for(std::chrono::seconds(5));
				system("cls");
				break;
			case 5:
				while (true) {
					std::cout << "Enter item name: ";
					std::cin >> name;

					bool validName = true;
					for (char c : name) {
						if (!isalpha(c)) {
							validName = false;
							break;
						}
					}

					if (validName) {
						break;  
					} else {
						std::cout << "Invalid item name. Please enter a name containing only alphabets.\n";
					}
				}


				while (true) {
					std::cout << "Enter price: ";
					if (std::cin >> price && price >= 0) {
						break;  
					} else {
						std::cin.clear();  
						std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
						std::cout << "Invalid input. Please enter a valid non-negative numeric value for the price.\n";
					}
				}

				menu.addItem(MenuItem(name, price));
				cout<<"item added successfully"<<endl;

				std::this_thread::sleep_for(std::chrono::seconds(3));
				system("cls");
				break;
			case 6:
				cout << "\nMenu:\n";
				menu.displayMenu();

	
				while (true) {
					cout << "Enter item index: ";
					if (cin >> index && index > 0) {
						break;  
					} else {
						cin.clear(); 
						cin.ignore(numeric_limits<streamsize>::max(), '\n');  
						cout << "Invalid input. Please enter a valid numeric value greater than 0 for item index.\n";
					}
				}

				menu.removeItem(index);
				std::this_thread::sleep_for(std::chrono::seconds(3));
				system("cls");
				break;
			case 7:
				cout << "Enter name: ";
			
				while (true) {
					cin>>rName;
					if (all_of(rName.begin(), rName.end(), ::isalpha)) {
						break; 
					} else {
						cout << "Invalid input. Name should only contain alphabets. Please enter a valid name.\n";
					}
				}

				while (true) {
					cout << "Enter contact no. (11 digits): ";
					cin >> rNum;

					if (cin.fail() || rNum.length() != 11 || !all_of(rNum.begin(), rNum.end(), ::isdigit)) {
						cin.clear();
						cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
						cout << "Invalid input. Please enter a valid 11-digit numeric value for the phone number.\n";
					} else {
						break;  
					}
				}

				manager.addRider(rName, rNum);
				cout << "Rider created successfully." << endl;
				std::this_thread::sleep_for(std::chrono::seconds(3));
				system("cls");
				break;
			case 8:
				cout<<"Enter name: ";
				while (true) {
					cin>>rName;
					if (all_of(rName.begin(), rName.end(), ::isalpha)) {
						break;  
					} else {
						cout << "Invalid input. Name should only contain alphabets. Please enter a valid name.\n";
					}
				}
				manager.removeRider(rName);
				std::this_thread::sleep_for(std::chrono::seconds(3));
				system("cls");
				break;
			case 9:
				manager.displayRiders();
				std::this_thread::sleep_for(std::chrono::seconds(3));
				system("cls");
				break;
			case 10:
				manager.displayAvailableRiders();
				std::this_thread::sleep_for(std::chrono::seconds(3));
				system("cls");
				break;
			case 11:
				salesInstance.DisplaySales();
				std::this_thread::sleep_for(std::chrono::seconds(3));
				system("cls");
				break;
			case 12:
				cout << "Displaying total Sales for the day and Existing the system.\n";
				system("cls");
				cout << "End of the day!" << endl;
				salesInstance.DisplaySales();
				std::this_thread::sleep_for(std::chrono::seconds(3));
				system("cls");
				break;
			default:
				cout << "Invalid choice. Please enter again.\n";
		}
		for(int i=0; i<manager.riders.size(); i++) {
			if(!manager.riders[i].available) {
				if(chrono::system_clock::now() >= manager.riders[i].availableTime) {
					manager.riders[i].available = true;
				}
			}
		}
	} while (choice != 12);

	return 0;
}