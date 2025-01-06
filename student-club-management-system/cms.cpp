#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstring>

using namespace std;

class User {
public:
	string prn, name, phone, email;
	char type;
	vector<string> uclubs;

	User()
	{
		prn = "0000000000";
		name = "user";
		phone = "1234567890";
		email = "abc@mitwpu.edu.in";
		type = 'n';
	}

	void displayDetails();
	void displayClubs();
	void giveFeedback();
	void applyToClub();
	void displayPrivateAnnouncements();
	void displayPublicAnnouncements();
	void volunteer();
	void changePassword();
};

class Event {
public:
	string ename, edate, cname;
};

class Club {
public:
	string cname, ctype, pres, vicepres;
	vector<string> members, event;

	void addMember();
	void removeMember();
	void displayMembers();
	void displayEvents();
	void displayReport();
	void makeAnnouncement();
	void displayFeedbacks();
	void displayVolunteerApplications();
	void createEventReport();
	void displayApplications();
};

class Admin : private User {
	void createUser();
	void editUser();
	void createClub();
	void editClub();
	void deleteClub();
	void registerEvent();
	void getEventReport();

	friend void adminMenu();
};

vector<User> users;
User currUser;
Club currClub;
vector<Event> events;
vector<Club> clubs;
Admin admin;
map<string, string> loginDetails;

void listClubs()
{
	vector<Club>::iterator c = clubs.begin();
	cout << endl << "Number of clubs: " << clubs.size() << endl << endl;
	cout << "Club Name\tClub Type" << endl;
	for (const Club& c : clubs) {
		cout << c.cname << "\t\t" << c.ctype << endl;
	}
}

void listEvents()
{
	vector<Event>::iterator e = events.begin();
	cout << endl << "Number of events: " << events.size() << endl << endl;
	cout << "Event Date\tHost Club\tEvent Name" << endl;
	for (const Event& e : events) {
		cout << e.edate << '\t' << e.cname << "\t\t" << e.ename << endl;
	}
}

void displayClubDetails()
{
	listClubs();
	string cname;
	cout << endl << "Enter the name of the club you want to view: ";
	cin >> cname;
	vector<Club>::iterator c = find_if(clubs.begin(), clubs.end(), [cname](const Club& obj) {
		return obj.cname == cname;
	});

	if (c == clubs.end()) {
		cout << endl << "No club found by the name '" << cname << "'." << endl;
		return;
	}

	cout << endl << "Club name: " << c->cname;
	cout << endl << "Club type: " << c->ctype;
	cout << endl << "Number of members: " << c->members.size();
	cout << endl << "President: ";
	string prn = c->pres;
	vector<User>::iterator u = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});
	cout << u->name << endl << "Events: " << endl;
	c->displayEvents();
}

void User::displayDetails()
{
	cout << endl << "Displaying user details:" << endl << endl;
	cout << "Name: " << currUser.name << endl;
	cout << "PRN: " << currUser.prn << endl;
	cout << "Phone no.: " << currUser.phone << endl;
	cout << "Email: "  << currUser.email << endl;
}

void User::displayClubs()
{
	if (uclubs.empty()) {
       	cout << "You are not a member of any club." << endl;
       	return;
    }

    cout << endl << "You are a member of the following clubs:" << endl;

	for (string element : uclubs) {
		cout << element << endl;
	}
}

void User::giveFeedback()
{
	if (uclubs.empty()) {
		cout << endl << "You are not a member of any club, so you cannot provide any feedback." << endl;
		return;
	}

	displayClubs();

	string cname;
	cout << endl << "Enter the name of the club you want to give feedback for: ";
	cin >> cname;

	vector<string>::iterator c = find(uclubs.begin(), uclubs.end(), cname);

	if (c == uclubs.end()) {
		cout << endl << "You are not a member of the club '" << cname << "'." << endl;
		return;
	}

	cout << "Enter your feedback for the club '" << cname << "' (Enter 'quit' on a new line to stop):" << endl;
	string filename = cname + "feedback.txt";
	string feedback;

	fstream opf(filename, ios::out | ios::app);
	opf << currUser.prn << ":" << endl;

	while (getline(cin, feedback) && feedback != "quit")
		opf << feedback << endl;

	opf << "--------------------" << endl;
	opf.close();

	cout << endl << "Your feedback has been recorded." << endl;
}

void User::applyToClub()
{
	listClubs();
	string cname;
	cout << endl << "Enter the name of the club you want to apply to: ";
	cin >> cname;
	vector<Club>::iterator c = find_if(clubs.begin(), clubs.end(), [cname](const Club& obj) {
		return obj.cname == cname;
	});

	if (c == clubs.end()) {
		cout << endl << "No club found with the name '" << cname << "'." << endl;
		return;
	}

	string filename = cname + "registrations.txt";

	fstream opf(filename, ios::out | ios::app);
	opf << currUser.prn << endl;
	opf.close();
	cout << endl << "You have successfully registered for the club '" << cname << "'." << endl;
	cout << "The club will contact you for further processes." << endl;
}

void User::displayPrivateAnnouncements()
{
	if (uclubs.empty()) {
		cout << endl << "You are not a member of any club, so you cannot provide any feedback." << endl;
		return;
	}

	displayClubs();
		
	string cname;
	cout << endl << "Enter the name of the club whose announcements you want to display: ";
	cin >> cname;

	vector<string>::iterator c = find(uclubs.begin(), uclubs.end(), cname);

	if (c == uclubs.end()) {
		cout << endl << "You are not a member of the club '" << cname << "'." << endl;
		return;
	}

	string filename = cname + "announcements.txt";
	string data;

	fstream ipf(filename);
	cout << endl << "Displaying all private announcements of '" << cname << "':" << endl << endl;

	while (getline(ipf, data))
		cout << data << endl;

	ipf.close();
}

void User::displayPublicAnnouncements()
{
	string data;
	fstream ipf("publicannouncements.txt");
	cout << endl << "Displaying all public announcements:" << endl << endl;

	while (getline(ipf, data))
		cout << data << endl;
}

void User::volunteer()
{
	listEvents();
	string ename;
	cout << endl << "Enter the name of event you want to volunteer for: ";
	cin >> ename;

	vector<Event>::iterator e = find_if(events.begin(), events.end(), [ename](const Event& obj) {
		return obj.ename == ename;
	});

	if (e == events.end()) {
		cout << endl << "Event not found." << endl;
		return;
	}

	string filename = ename + "volapplications.txt";
	fstream opf(filename, ios::out | ios::app);
	opf << currUser.prn << " - " << currUser.name << " - " << currUser.phone << endl;
	opf.close();

	cout << endl << "You have successfully registered to volunteer for '" << ename << "'." << endl;
}

void User::changePassword()
{
	string password;
	map<string, string>::iterator uname = loginDetails.find(currUser.prn);
	cout << endl << "Enter current password: ";
	cin >> password;
	if (password != uname->second) {
		cout << endl << "Incorrect password.";
		return;
	}
	cout << endl << "Enter new password: ";
	cin >> uname->second;
	cout << endl << "Password has been updated successfully." << endl << endl;
}

void Club::addMember()
{
	string prn;
	cout << endl << "Enter PRN of the member you want to add: ";
	cin >> prn;
	vector<User>::iterator u = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});

	if (u == users.end()) {
		cout << endl << "User with PRN '" << prn << "' not found." << endl;
		return;
	}

	members.push_back(u->prn);
	u->uclubs.push_back(cname);

	cout << endl << "New member added." << endl << "Member name: " << u->name << endl;
}

void Club::removeMember()
{
	string prn;
	cout << endl << "Enter PRN of the member you want to remove: ";
	cin >> prn;

	vector<string>::iterator m = find(members.begin(), members.end(), prn);

	if (m == members.end()) {
		cout << endl << "No member found with PRN '" << prn << "'." << endl;
		return;
	}

	vector<User>::iterator u = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});
	char choice;
	cout << "You want to remove '" << u->name << "' from your club. Do you want to proceed? (Y/n): ";
	cin >> choice;

	if (choice == 'Y' || choice == 'y') {
		members.erase(m);
		vector<string>::iterator c = find(u->uclubs.begin(), u->uclubs.end(), cname);
		u->uclubs.erase(c);
		cout << endl << u->name << " has been removed from the club." << endl;
	}
	else
		cout << endl << u->name << " was not removed." << endl;
}

void Club::displayMembers()
{
	string prn = pres;
	vector<User>::iterator u = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});

	cout << endl << "Number of members: " << members.size() << endl << endl;
	cout << "PRN\t\tName\tPhone no.\tRole" << endl;
	cout << pres << '\t' << u->name << '\t' << u->phone << '\t' << "President" << endl;

	prn = vicepres;
	u = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});
	cout << vicepres << '\t' << u->name << '\t' << u->phone << '\t' << "Vice President" << endl;
	
	for (const string& mem : members) {
		prn = mem;
		u = find_if(users.begin(), users.end(), [prn](const User& obj) {
			return obj.prn == prn;
		});
		if (u->type == 'n')
			cout << prn << '\t' << u->name << '\t' << u->phone << '\t' << "Member" << endl;
	}
}

void Club::displayEvents()
{
	int count = 0;
	cout << endl << "Date\t\tName" << endl;
	for (const Event& eve : events) {
		if (eve.cname == cname) {
			cout << eve.edate << '\t' << eve.ename << endl;
			count++;
		}
	}
	cout << endl << "Number of events: " << count << endl;
}

void Club::displayReport()
{
	displayMembers();
	cout << endl << "Events:" << endl;
	displayEvents();
}

void Club::makeAnnouncement()
{
	string type, announcement, filename;
	cout << endl << "Enter the type of announcement you wish to make (private/public): ";
	cin >> type;

	if (type == "private")
		filename = cname + "announcements.txt";
	else if (type == "public")
		filename = "publicannouncements.txt";

	fstream opf(filename, ios::out | ios::app);

	if (type == "public")
		opf << cname << " on ";

	cout << endl << "Enter date of announcement (dd-mm-yyyy): ";
	cin >> announcement;
	announcement = announcement + ":";
	opf << announcement << endl;

	cout << endl << "Enter the announcement (Enter 'quit' on a new line to stop):" << endl;
	while (getline(cin, announcement) && announcement != "quit")
		opf << announcement << endl;
	opf << "--------------------" << endl;
	opf.close();
}

void Club::displayFeedbacks()
{
	string filename = cname + "feedback.txt", data;
	fstream ipf(filename);

	while (getline(ipf, data))
		cout << data << endl;
	
	ipf.close();
}

void Club::displayVolunteerApplications()
{
	displayEvents();
	string ename, data;
	cout << endl << "Enter event name to create a report for: ";
	cin >> ename;

	vector<Event>::iterator e = find_if(events.begin(), events.end(), [ename](const Event& obj){
		return obj.ename == ename;
	});

	if (e == events.end() || e->cname != cname) {
		cout << endl << "Event not found for your club." << endl;
		return;
	}

	string filename = ename + "volapplications.txt";
	fstream ipf(filename);

	while (getline(ipf, data))
		cout << data << endl;
	
	ipf.close();
}

void Club::createEventReport()
{
	displayEvents();
	string ename, data;
	cout << endl << "Enter event name to create a report for: ";
	cin >> ename;

	vector<Event>::iterator e = find_if(events.begin(), events.end(), [ename](const Event& obj){
		return obj.ename == ename;
	});

	if (e == events.end() || e->cname != cname) {
		cout << endl << "Event not found for your club." << endl;
		return;
	}

	string filename = ename + "report.txt";
	fstream opf(filename, ios::out);

	opf << "Event name: " << e->ename << endl;
	opf << "Host club: " << e->cname << endl;
	
	cout << endl << "Enter number of attendees: ";
	cin >> data;

	opf << "Number of attendees: " << data << endl << endl;

	char choice;
	cout << endl << "Were there any guests for this event? (Y/n): ";
	cin >> choice;

	if (choice == 'y' || choice == 'Y') {
		opf << "Guest list:" << endl << endl;
		opf << "Guest name\t\tDesignation/Institute" << endl;
	}

	while (choice == 'y' || choice == 'Y') {
		cout << endl << "Enter the name of the guest: ";
		cin.ignore();
		getline(cin, data);
		opf << data << "\t";
		cout << "Enter the designation/institute of the guest: ";
		getline(cin, data);
		opf << data << endl;

		cout << endl << "Do you want to add another guest? (Y/n): ";
		cin >> choice;
	}

	cout << endl << "Did this event involve volunteers (other than club members)? (Y/n): ";
	cin >> choice;

	if (choice == 'y' || choice == 'Y') {
		opf << endl << "List of volunteers: " << endl << endl;
		opf << "PRN\tName" << endl;
	}

	vector<User>::iterator u;
	string prn;

	while (choice == 'y' || choice == 'Y') {
		cout << endl << "Enter the PRN of the volunteer: ";
		cin >> prn;

		u = find_if(users.begin(), users.end(), [prn](const User& obj){
			return obj.prn == prn;
		});

		if (u == users.end()) {
			cout << endl << "No user found with given PRN." << endl;
			continue;
		}

		data = u->name;
		opf << prn << "\t" << data << endl;

		cout << endl << "Do you want to add another volunteer? (Y/n): ";
		cin >> choice;
	}

	cout << endl << "Event report created successfully." << endl;
}

void Club::displayApplications()
{
	string data, filename = cname + "registrations.txt";
	fstream ipf(filename);
	cout << endl << "Displaying all applications to the club:" << endl << endl;

	while (getline(ipf, data))
		cout << data << endl;
}

void Admin::createUser()
{
	User temp;
	string prn, password;
	cout << endl << "Enter PRN: ";
	cin >> prn;
	vector<User>::iterator u = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});

	if (u != users.end()) {
		cout << endl << "User already exists." << endl;
		return;
	}

	temp.prn = prn;
	cout << "Enter name: ";
	cin >> temp.name;
	cout << "Enter phone number: ";
	cin >> temp.phone;
	cout << "Enter email: ";
	cin >> temp.email;
	cout << "Enter login password for new user: ";
	cin >> password;
	users.push_back(temp);
	loginDetails.insert(make_pair(prn ,password));
	cout << endl << "User has been created successfully." << endl;
}

void Admin::editUser()
{
	string prn;
	cout << endl << "Enter PRN of user to edit: ";
	cin >> prn;
	vector<User>::iterator u = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});
	
	if (u != users.end()) {
		cout << "User found." << endl;
		cout << endl << "Current name: " << u->name << endl;
		map<string, string>::iterator uname = loginDetails.find(prn);
		int choice;
		string password;
		while (1) {
			cout << endl << "What would you like to edit?\n1 - PRN\n2 - Name\n3 - Phone no.\n4 - Email\n5 - Exit\n\nYour choice: ";
			cin >> choice;
			switch (choice) {
			case 1:
				cout << endl << "Current PRN: " << u->prn << endl;
				cout << "Enter new PRN: ";
				cin >> u->prn;
				password = uname->second;
				loginDetails.erase(uname->first);
				loginDetails.insert(make_pair(u->prn, password));
				cout << "PRN has been updated to '" << u->prn << "' successfully." << endl;
				break;
			case 2:
				cout << endl << "Current name: " << u->name << endl;
				cout << "Enter new name: ";
				cin >> u->name;
				cout << "Name has been updated to '" << u->name << "' successfully." << endl;
				break;
			case 3:
				cout << endl << "Current phone no.: " << u->phone << endl;
				cout << "Enter new phone no.: ";
				cin >> u->phone;
				cout << "Phone no. has been updated to '" << u->phone << "' successfully." << endl;
				break;
			case 4:
				cout << endl << "Current email: " << u->email << endl;
				cout << "Enter new email: ";
				cin >> u->email;
				cout << "Email has been updated to '" << u->email << "' successfully." << endl;
				break;
			case 5:
				cout << endl << "Changes saved." << endl;
				return;
			default:
				cout << endl << "Invalid input! Try again." << endl;
			}
		}			
	}
	else {
		cout << endl << "User not found." << endl;
	}
}

void Admin::createClub()
{
	Club temp;
	string prn, cname;
	cout << endl << "Enter club name: ";
	cin >> cname;
	vector<Club>::iterator c = find_if(clubs.begin(), clubs.end(), [cname](const Club& obj){
		return obj.cname == cname;
	});

	if (c != clubs.end()) {
		cout << endl << "Club already exists." << endl;
		return;
	}

	temp.cname = cname;
	cout << "Enter club type: ";
	cin >> temp.ctype;

	cout << "Enter PRN of President: ";
	cin >> prn;
	vector<User>::iterator u1 = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});

	if(u1 == users.end()) {
		cout << endl << "President PRN not found. Try again." << endl;
		return;
	}

	temp.members.push_back(u1->prn);
	temp.pres = u1->prn;
	cout << "Name of President: " << u1->name << endl;

	cout << "Enter PRN of Vice President: ";
	cin >> prn;
	vector<User>::iterator u2 = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});

	if(u2 == users.end()) {
		cout << endl << "Vice President PRN not found. Try again." << endl;
		return;
	}

	temp.members.push_back(u2->prn);
	temp.vicepres = u2->prn;
	cout << "Name of Vice President: " << u2->name << endl;

	cout << "Enter PRN of third member: ";
	cin >> prn;
	vector<User>::iterator u3 = find_if(users.begin(), users.end(), [prn](const User& obj) {
		return obj.prn == prn;
	});

	if(u3 == users.end()) {
		cout << endl << "Member PRN not found. Try again." << endl;
		return;
	}

	temp.members.push_back(u3->prn);
	cout << "Name of new member: " << u3->name;

	u1->type = 'p';
	u1->uclubs.push_back(temp.cname);
	u2->type = 'v';
	u2->uclubs.push_back(temp.cname);
	u3->uclubs.push_back(temp.cname);
	clubs.push_back(temp);
	cout << endl << "The club '" << temp.cname << "' has been created successfully." << endl;
}

void Admin::editClub()
{
	string cname;
	listClubs();
	cout << endl << "Enter the name of the club you want to edit: ";
	cin >> cname;
	vector<Club>::iterator c = find_if(clubs.begin(), clubs.end(), [cname](const Club& obj) {
		return obj.cname == cname;
	});

	if (c == clubs.end()) {
		cout << endl << "No club found with the name '" << cname << "'." << endl;
		return;
	}

	int choice;
	while (1) {
		cout << endl << "What do you want to change?\n1 - President\n2 - Vice President\n3 - Exit\n\nYour choice: ";
		cin >> choice;
		string prn;
		int count;
		vector<User>::iterator u, newu;
		switch (choice) {
		case 1:
			cout << endl << "Current President PRN: " << c->pres;
			prn = c->pres;
			u = find_if(users.begin(), users.end(), [prn](const User& obj) {
				return obj.prn == prn;
			});
			cout << endl << "Current President Name: " << u->name << endl;

			cout << endl << "Enter PRN of new President: ";
			cin >> prn;
			newu = find_if(users.begin(), users.end(), [prn](const User& obj) {
				return obj.prn == prn;
			});
			if (newu == users.end()) {
				cout << "New President PRN not found. Try again.";
				continue;
			}
			count = 0;
			for (const string& mem : c->members) {
				if (mem == newu->prn) {
					count++;
				}
			}
			if (count == 0) {
				cout << endl << "New President must be a member of the club." << endl;
				continue;
			}
			u->type = 'n';
			cout << "Name of new President: " << newu->name << endl;
			newu->type = 'p';
			c->pres = newu->prn;
			break;

		case 2:
			cout << endl << "Current Vice President PRN: " << c->vicepres;
			prn = c->vicepres;
			u = find_if(users.begin(), users.end(), [prn](const User& obj) {
				return obj.prn == prn;
			});
			cout << endl << "Current Vice President Name: " << u->name << endl;

			cout << endl << "Enter PRN of new Vice President: ";
			cin >> prn;
			newu = find_if(users.begin(), users.end(), [prn](const User& obj) {
				return obj.prn == prn;
			});
			if (newu == users.end()) {
				cout << "New Vice President PRN not found. Try again.";
				continue;
			}
			count = 0;
			for (const string& mem : c->members) {
				if (mem == newu->prn) {
					count++;
				}
			}
			if (count == 0) {
				cout << endl << "New Vice President must be a member of the club." << endl;
				continue;
			}
			u->type = 'n';
			cout << "Name of new Vice President: " << newu->name << endl;
			newu->type = 'v';
			c->vicepres = newu->prn;
			break;
		case 3:
			cout << endl << "Changes saved." << endl;
			return;
		default:
			cout << endl << "Invalid input! Try again." << endl;
		}
	}
}

void Admin::deleteClub()
{
	string cname;
	listClubs();
	cout << endl << "Enter the name of the club you want to delete: ";
	cin >> cname;
	vector<Club>::iterator c = find_if(clubs.begin(), clubs.end(), [cname](const Club& obj) {
		return obj.cname == cname;
	});

	if (c == clubs.end()) {
		cout << endl << "Selected club doesn't exist." << endl;
		return;
	}

	char choice;
	cout << endl << "You want to delete the club '" << c->cname << "'. Do you want to proceed? (Y/n): ";
	cin >> choice;
	if (choice == 'y' || choice == 'Y') {
		vector<User>::iterator u;
		vector<string>::iterator delc;
		string prn;
		for (string mem : c->members) {
			prn = mem;
			u = find_if(users.begin(), users.end(), [prn](const User& obj) {
				return obj.prn == prn;
			});
			delc = find(u->uclubs.begin(), u->uclubs.end(), cname);
			u->uclubs.erase(delc);
			if (u->prn == c->pres || u->prn == c->vicepres)
				u->type = 'n';
		}
		clubs.erase(c);
		cout << endl << "The club '" << cname << "' has been deleted successfully." << endl;
	}
	else
		cout << endl << "The club '" << cname << "' was not deleted." << endl;
}

void Admin::registerEvent()
{
	Event temp;
	string cname;
	cout << endl << "Enter club name: ";
	cin >> cname;

	vector<Club>::iterator c = find_if(clubs.begin(), clubs.end(), [cname](const Club& obj) {
		return obj.cname == cname;
	});

	if (c == clubs.end()) {
		cout << endl << "Club not found." << endl;
		return;
	}

	temp.cname = cname;
	cout << "Enter event name: ";
	cin >> temp.ename;
	cout << "Enter event date (dd-mm-yyyy): ";
	cin >> temp.edate;

	c->event.push_back(temp.ename);
	events.push_back(temp);
	cout << endl << "Event has been registered succesfully" << endl;
}

void Admin::getEventReport()
{
	listEvents();
	string ename, data;
	cout << endl << "Enter event name to display the report for: ";
	cin >> ename;

	vector<Event>::iterator e = find_if(events.begin(), events.end(), [ename](const Event& obj){
		return obj.ename == ename;
	});

	if (e == events.end()) {
		cout << endl << "Event not found." << endl;
		return;
	}

	string filename = ename + "report.txt";
	fstream ipf(filename);

	while(getline(ipf, data)) {
		cout << data << endl;
	}
}

string login()
{
	string username, password, prn;
	map<string, string>::iterator uname;
	bool success = false;
	vector<User>::iterator u;
	vector<Club>::iterator c;
	while(!success) {
		cout << endl << endl << "Enter username/PRN: ";
		cin >> username;
		uname = loginDetails.find(username);
		if (uname == loginDetails.end()) {
			cout << endl << "Username not found. Try again." << endl;
			continue;
		}

		cout << "Enter password: ";
		cin >> password;
		if (password == uname->second) {
			success = true;
			if (username == "admin") {
				cout << endl << "Welcome admin!" <<  endl;
				return "admin";
			}
			else {
				prn = username;
				u = find_if(users.begin(), users.end(), [prn](const User& obj) {
					return obj.prn == prn;
				});
				
				try {
					if (u == users.end())
						throw(u);
					else {
						currUser = *u;
						cout << endl << endl << "Welcome, " << currUser.name << "!" << endl;
					}
				}
				catch (vector<User>::iterator u) {
					cout << endl << "No user found." << endl;
					return "";
				}
				if (u->type == 'n')
					return "user";
				else if (u->type == 'p' || u->type == 'v') {
					c = find_if(clubs.begin(), clubs.end(), [username](const Club& obj) {
						return obj.pres == username;
					});
					currClub = *c;
					return "president";
				}
			}
		}
		else
			cout << endl << "Incorrect password! Try again." << endl;
	}
	return "";
}

void userMenu()
{
	int choice;
	while (1) {
		cout << endl << "Choose operation:" << endl;
		cout << "1 - Display user details" << endl;
		cout << "2 - Display clubs list" << endl;
		cout << "3 - Display events list" << endl;
		cout << "4 - Display a club's details" << endl;
		cout << "5 - Display your clubs" << endl;
		cout << "6 - Apply to a club" << endl;
		cout << "7 - Show public announcements" << endl;
		cout << "8 - Show announcements of your clubs" << endl;
		cout << "9 - Volunteer for an event" << endl;
		cout << "10 - Give feedback to a club" << endl;
		cout << "11 - Change password" << endl;
		cout << "12 - Logout" << endl << endl;
		cout << "Your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			currUser.displayDetails();
			break;

		case 2:
			listClubs();
			break;

		case 3:
			listEvents();
			break;

		case 4:
			displayClubDetails();
			break;

		case 5:
			currUser.displayClubs();
			break;

		case 6:
			currUser.applyToClub();
			break;

		case 7:
			currUser.displayPublicAnnouncements();
			break;

		case 8:
			currUser.displayPrivateAnnouncements();
			break;

		case 9:
			currUser.volunteer();
			break;

		case 10:
			currUser.giveFeedback();
			break;

		case 11:
			currUser.changePassword();
			break;

		case 12:
			cout << endl << "Logging out from " << currUser.prn << "." << endl;
			return;

		default:
			cout << endl << "Invalid input! Try again." << endl;
		}
	}
}

void presidentMenu()
{
	int choice;
	string cname;
	vector<Club>::iterator c;
	while (1) {
		cout << endl << "Choose operation:" << endl;
		cout << "1 - Display user details" << endl;
		cout << "2 - Display clubs list" << endl;
		cout << "3 - Display events list" << endl;
		cout << "4 - Display a club's details" << endl;
		cout << "5 - Display your clubs" << endl;
		cout << "6 - Apply to a club" << endl;
		cout << "7 - Show public announcements" << endl;
		cout << "8 - Show announcements of your clubs" << endl;
		cout << "9 - Volunteer for an event" << endl;
		cout << "10 - Give feedback to a club" << endl;
		cout << "11 - Add a member to '" << currClub.cname << "'" << endl;
		cout << "12 - Remove a member from '" << currClub.cname << "'" << endl;
		cout << "13 - Make an announcement for '" << currClub.cname << "'" << endl;
		cout << "14 - Create an event report for '" << currClub.cname << "'" << endl;
		cout << "15 - Display feedbacks for '" << currClub.cname << "'" << endl;
		cout << "16 - Display volunteer applications for an event of '" << currClub.cname << "'" << endl;
		cout << "17 - Display club applications for '" << currClub.cname << "'" << endl;
		cout << "18 - Change password" << endl;
		cout << "19 - Logout" << endl << endl;
		cout << "Your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			currUser.displayDetails();
			break;

		case 2:
			listClubs();
			break;

		case 3:
			listEvents();
			break;

		case 4:
			displayClubDetails();
			break;

		case 5:
			currUser.displayClubs();
			break;

		case 6:
			currUser.applyToClub();
			break;

		case 7:
			currUser.displayPublicAnnouncements();
			break;

		case 8:
			currUser.displayPrivateAnnouncements();
			break;

		case 9:
			currUser.volunteer();
			break;

		case 10:
			currUser.giveFeedback();
			break;

		case 11:
			currClub.addMember();
			break;

		case 12:
			currClub.removeMember();
			break;

		case 13:
			currClub.makeAnnouncement();
			break;

		case 14:
			currClub.createEventReport();
			break;

		case 15:
			currClub.displayFeedbacks();
			break;

		case 16:
			currClub.displayVolunteerApplications();
			break;

		case 17:
			currClub.displayApplications();
			break;

		case 18:
			currUser.changePassword();
			break;

		case 19:
			cout << endl << "Logging out from " << currUser.prn << "." << endl;
			return;

		default:
			cout << endl << "Invalid input! Try again." << endl;
		}
	}
}

void adminMenu()
{
	int choice;
	string cname;
	vector<Club>::iterator c;
	while (1) {
		cout << endl << "Choose operation:" << endl;
		cout << "1 - Create new user" << endl;
		cout << "2 - Edit a user" << endl;
		cout << "3 - Create new club" << endl;
		cout << "4 - Edit a club" << endl;
		cout << "5 - Delete a club" << endl;
		cout << "6 - Display clubs list" << endl;
		cout << "7 - Display a club's details" << endl;
		cout << "8 - Display a club's report" << endl;
		cout << "9 - Register new event" << endl;
		cout << "10 - Display events list" << endl;
		cout << "11 - Display event report" << endl;
		cout << "12 - Logout" << endl << endl;
		cout << "Your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			admin.createUser();
			break;

		case 2:
			admin.editUser();
			break;

		case 3:
			admin.createClub();
			break;

		case 4:
			admin.editClub();
			break;

		case 5:
			admin.deleteClub();
			break;

		case 6:
			listClubs();
			break;

		case 7:
			displayClubDetails();
			break;

		case 8:
			cout << endl << "Enter the name of the club whose report you want to view: ";
			cin >> cname;

			c = find_if(clubs.begin(), clubs.end(), [cname](const Club& obj) {
				return obj.cname == cname;
			});

			if (c == clubs.end()) {
				cout << endl << "No club found with the name '" << cname << "'." << endl;
				continue;
			}

			c->displayReport();
			break;

		case 9:
			admin.registerEvent();
			break;

		case 10:
			listEvents();
			break;

		case 11:
			admin.getEventReport();
			break;

		case 12:
			cout << endl << "Logging out from admin." << endl;
			return;

		default:
			cout << endl << "Invalid input! Try again." << endl;
		}
	}
}

void loadData()
{
	string filename, data1, data2, cname, name;
	Club tclub;
	Event tevent;
	User tuser;
	vector<Club>::iterator c;
	
	// loading login details
	
	filename = "login.txt";

	fstream ipf(filename);

	while (getline(ipf, data1)) {
		getline(ipf, data2);
		loginDetails.insert({data1, data2});
	}

	ipf.close();

	// loading clubs

	filename = "clubs.txt";

	ipf.open(filename);

	while (getline(ipf, data1)) {
		tclub.cname = data1;
		getline(ipf, data1);
		tclub.ctype = data1;
		getline(ipf, data1);
		tclub.pres = data1;
		getline(ipf, data1);
		tclub.vicepres = data1;
		clubs.push_back(tclub);
	}

	ipf.close();

	// loading events

	filename = "events.txt";

	ipf.open(filename);

	while (getline(ipf, data1)) {
		tevent.ename = data1;
		name = data1;
		getline(ipf, data1);
		tevent.edate = data1;
		getline(ipf, data1);
		tevent.cname = data1;
		events.push_back(tevent);

		cname = data1;
		c = find_if(clubs.begin(), clubs.end(), [cname](const Club& obj){
			return obj.cname == cname;
		});

		c->event.push_back(name);
	}

	ipf.close();

	// loading users

	filename = "users.txt";

	ipf.open(filename);

	while (getline(ipf, data1)) {
		tuser.prn = data1;
		getline(ipf, data1);
		tuser.name = data1;
		getline(ipf, data1);
		tuser.phone = data1;
		getline(ipf, data1);
		tuser.email = data1;
		getline(ipf, data1);
		tuser.type = data1[0];
		tuser.uclubs.clear();

		while (getline(ipf, data1) && data1 != "--------------------") {
			tuser.uclubs.push_back(data1);

			cname = data1;
			c = find_if(clubs.begin(), clubs.end(), [cname](const Club& obj){
				return obj.cname == cname;
			});
			c->members.push_back(tuser.prn);
		}

		users.push_back(tuser);
	}

	ipf.close();
}

void saveData()
{
	string filename;

	// storing login details

	filename = "login.txt";

	fstream opf(filename, ios::out);

	for (auto username : loginDetails) {
		opf << username.first << endl << username.second << endl;
	}

	opf.close();

	// storing clubs

	filename = "clubs.txt";

	opf.open(filename, ios::out);

	for (const Club& c : clubs) {
		opf << c.cname << endl << c.ctype << endl << c.pres << endl << c.vicepres << endl;
	}

	opf.close();

	// storing events

	filename = "events.txt";

	opf.open(filename, ios::out);

	for (const Event& e : events) {
		opf << e.ename << endl << e.edate << endl << e.cname << endl;
	}

	opf.close();

	// storing users

	filename = "users.txt";

	opf.open(filename, ios::out);

	for (const User& u : users) {
		opf << u.prn << endl << u.name << endl << u.phone << endl << u.email << endl << u.type << endl;
		for (const string& cname : u.uclubs) {
			opf << cname << endl;
		}
		opf << "--------------------" << endl;
	}

	opf.close();
}

int main()
{
	loadData();
	string loginType;
	char choice;

	cout << endl << "---------------------------------------------------------------------------------------------------------------";
	cout << endl << "                                        STUDENT CLUBS MANAGEMENT SYSTEM                                        ";
	cout << endl << "---------------------------------------------------------------------------------------------------------------";

	do {
		loginType = login();

		if (loginType == "admin") 
			adminMenu();
		else if (loginType == "president")
			presidentMenu();
		else if (loginType == "user")
			userMenu();

		saveData();
		cout << endl << "Do you want to login again? (Y/n): ";
		cin >> choice;
	} while (choice == 'y' || choice == 'Y');

	return 0;
}