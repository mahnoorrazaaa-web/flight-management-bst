#include<iostream>
#include <string>		//for full names
#include <fstream>		//for read and write in files(file handling)
using namespace std;

struct flight {
	int ID;
	string source;
	string destination;
	string airline;
	int departureHour;
	int departureMinute;
	int arrivalHour;
	int arrivalMinute;
	double duration;
	double price;
	string flightstatue;
};
struct node{
	flight info;			//stores whole flight information
	node* left;				//left child
	node* right;			//right child
};
class flightSyestem {		//main class for syestem
	public:
	node* root;

	flightSyestem() {
		root = NULL;
	}
	node* AddFlight(node* root, flight newflight) {
		if (root == NULL) {
			node* newnode = new node;
			newnode->info = newflight;
			newnode->left = NULL;
			newnode->right = NULL;
			return newnode;
		}
		else if (newflight.ID < root->info.ID)
			root->left = AddFlight(root->left, newflight);
		else if (newflight.ID > root->info.ID)
			root->right = AddFlight(root->right, newflight);
		else
			cout << "duplicate ID" << endl;
		return root;
	}
	node* searchFlight(node* root, int id) {
		if (root == NULL) {
			return NULL;
		}
		else if (id == root->info.ID)
			return root;
		else if (id < root->info.ID) {

			return searchFlight(root->left, id);
		}
		else {
			return searchFlight(root->right, id); 
		}
	}

	node* findMin(node* root){
		while (root->left != NULL){
			root = root->left;
		}
		return root;
	}

	node* deleteNode(node* root, int id){
		if (root == NULL){
			return NULL;
		}
		if (id < root->info.ID){
			root->left = deleteNode(root->left, id);
		}
		else if (id > root->info.ID){
			root->right = deleteNode(root->right, id);
		}
		else{
			if (root->left == NULL && root->right == NULL){	//  Leaf node(has no child node)
				delete root;
				return NULL;
			}
			else if (root->left == NULL){				//  Only right child
				node* temp = root->right;
				delete root;
				return temp;
			}
			else if (root->right == NULL)	{		//only left child
				node* temp = root->left;
				delete root;
				return temp;
			}
			else{
					//2 children
				node* temp = findMin(root->right);
				root->info = temp->info;
				root->right = deleteNode(root->right, temp->info.ID);
			}
		}
		return root;
	}

	void inorder(node* root){
		if (root == NULL) {
			return;
		}
		inorder(root->left);
		cout << "flight ID: " << root->info.ID << endl;
		cout << "source: " << root->info.source << endl;
		cout << "destination: " << root->info.destination << endl;
		cout << "airline: " << root->info.airline << endl;
		cout << "departure: " << root->info.departureHour <<" : "<< root->info.departureMinute << endl;
		cout << "arrival: " << root->info.arrivalHour << " : " << root->info.arrivalMinute << endl;
		cout << "duration: " << root->info.duration << endl;
		cout << "ticket price: " << root->info.price << endl;
		cout << "flight status: " << root->info.flightstatue << endl;
		inorder(root->right);
	}

	string minutesToTime(int totalMinutes){

		totalMinutes = totalMinutes % 1440;		//1440 minutes in a day.
		int hours = totalMinutes / 60;
		int minutes = totalMinutes % 60;
		string result = "";

		if (hours < 10) {
			result += "0";

			result += to_string(hours);
			result += ":";
		}
		if (minutes < 10) {
			result += "0";
			result += to_string(minutes);
		}
		return result;
	}
	void calculateETA(node* root) {
		if (root == NULL)
			return;

		calculateETA(root->left);

		int departure = root->info.departureHour * 60 + root->info.departureMinute;
		int arrival = root->info.arrivalHour * 60 + root->info.arrivalMinute;

		if (arrival < departure)
			arrival += 1440;

		int duration = root->info.duration * 60;

		int eta = departure + duration;

		cout << "Flight ID: " << root->info.ID << endl;;
		cout << "Expected Arrival Time: " << minutesToTime(eta) << endl;

		if (eta == arrival)
			cout << "Status: On-Time" << endl;
		else if (eta < arrival)
			cout << "Status: Early" << endl;
		else
			cout << "Status: Delayed" << endl;

		calculateETA(root->right);
	}
	void saveFlights(node* root, ofstream& file) {

		if (root == NULL)
			return;

		saveFlights(root->left, file);

		file << root->info.ID << ",";
		file << root->info.source << ",";
		file << root->info.destination << ",";
		file << root->info.airline << ",";
		file << root->info.departureHour << ",";
		file << root->info.departureMinute << ",";
		file << root->info.arrivalHour << ",";
		file << root->info.arrivalMinute << ",";
		file << root->info.duration << ",";
		file << root->info.price << ",";
		file << root->info.flightstatue << endl;

		saveFlights(root->right, file);
	}
	void loadFlights() {

		ifstream file("flights.txt");
		if (!file) {
			cout << "No saved flight file found!" << endl;
			return;
		}
		flight f;
		while (file >> f.ID) {
			file.ignore();

			getline(file, f.source, ',');
			getline(file, f.destination, ',');
			getline(file, f.airline, ',');

			file >> f.departureHour;
			file.ignore();

			file >> f.departureMinute;
			file.ignore();

			file >> f.arrivalHour;
			file.ignore();

			file >> f.arrivalMinute;
			file.ignore();

			file >> f.duration;
			file.ignore();

			file >> f.price;
			file.ignore();

			getline(file, f.flightstatue);

			root = AddFlight(root, f);
		}

		file.close();
		cout << "Flights loaded successfully!" << endl;
	}
	
		
};
int main() {
	flightSyestem s;

	s.loadFlights();

	int choice;

	do {
		cout << "\n========== FLIGHT MANAGEMENT SYSTEM ==========" << endl;
		cout << "1. Add Flight" << endl;
		cout << "2. Search Flight" << endl;
		cout << "3. Delete Flight" << endl;
		cout << "4. Display All Flights" << endl;
		cout << "5. Calculate ETA" << endl;
		cout << "6. Save Flights" << endl;
		cout << "7. Load Flights" << endl;
		cout << "0. Exit" << endl;

		cout << "\nEnter your choice: ";
		cin >> choice;

		switch (choice) {

		case 1: {
			flight f;

			cout << "Enter Flight ID: ";
			cin >> f.ID;
			cin.ignore();

			cout << "Enter Source: ";
			getline(cin, f.source);

			cout << "Enter Destination: ";
			getline(cin, f.destination);

			cout << "Enter Airline: ";
			getline(cin, f.airline);

			cout << "Enter Departure Hour: ";
			cin >> f.departureHour;

			cout << "Enter Departure Minute: ";
			cin >> f.departureMinute;

			cout << "Enter Arrival Hour: ";
			cin >> f.arrivalHour;

			cout << "Enter Arrival Minute: ";
			cin >> f.arrivalMinute;

			cout << "Enter Duration in hours: ";
			cin >> f.duration;

			cout << "Enter Ticket Price: ";
			cin >> f.price;
			cin.ignore();

			cout << "Enter Flight Status: ";
			getline(cin, f.flightstatue);

			s.root = s.AddFlight(s.root, f);

			break;
		}

		case 2: {
			int id;

			cout << "Enter Flight ID to search: ";
			cin >> id;

			node* result = s.searchFlight(s.root, id);

			if (result == NULL) {
				cout << "Flight not found!" << endl;
			}
			else {
				cout << "\nFlight Found!" << endl;
				cout << "Flight ID: " << result->info.ID << endl;
				cout << "Source: " << result->info.source << endl;
				cout << "Destination: " << result->info.destination << endl;
				cout << "Airline: " << result->info.airline << endl;
				cout << "Departure: " << result->info.departureHour << ":" << result->info.departureMinute << endl;
				cout << "Arrival: " << result->info.arrivalHour << ":" << result->info.arrivalMinute << endl;
				cout << "Duration: " << result->info.duration << endl;
				cout << "Price: " << result->info.price << endl;
				cout << "Status: " << result->info.flightstatue << endl;
			}

			break;
		}

		case 3: {
			int id;

			cout << "Enter Flight ID to delete: ";
			cin >> id;

			if (s.searchFlight(s.root, id) == NULL) {
				cout << "Flight not found!" << endl;
			}
			else {
				s.root = s.deleteNode(s.root, id);
				cout << "Flight deleted successfully!" << endl;
			}

			break;
		}

		case 4:
			if (s.root == NULL) {
				cout << "No flights available!" << endl;
			}
			else {
				s.inorder(s.root);
			}
			break;

		case 5:
			if (s.root == NULL) {
				cout << "No flights available!" << endl;
			}
			else {
				s.calculateETA(s.root);
			}
			break;

		case 6: {
			ofstream file("flights.txt");

			if (file) {
				s.saveFlights(s.root, file);
				file.close();
				cout << "Flights saved successfully!" << endl;
			}
			else {
				cout << "Error opening file!" << endl;
			}

			break;
		}

		case 7:
			s.loadFlights();
			break;

		case 0:
			cout << "Exiting Flight Management System!" << endl;
			break;

		default:
			cout << "Invalid choice! Try again." << endl;
		}

	} while (choice != 0);

	return 0;
}