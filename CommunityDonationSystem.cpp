//-------------------------------------------
//Software Engineering Group Project
//Community Donation System
//A Command Line Interface (CLI) program
//For managing and tracking item donations
//-------------------------------------------



//include necessary includes
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cctype>
#include <limits>

//standard using namespace
using namespace std;

// set up each individual data structure


//data structure for a User
//Each user will require an email, password, name, and role(User, Admin, Tester)
struct User {
    string email;
    string password;
    string name;
    string role; //Can be "User", "Admin", "Tester"
};

//Data structure for an item posted to be viewed and collected
//Each item will contain an Id, a title, a category
//In this case: Furniture, Clothing, Toys & Games, Electronics, or Other
//A condition,  description, location, status(Available, pending pickup, claimed)
//and a poster(User who posted the item)
struct Item {
    int id;
    string title;
    string category; //Can be Furniture, Clothing, Toys & Games, Electronics, Other
    string condition;
    string description;
    string location;
    string status; //Can be set to "Available", "Pending Pickup", "Claimed"
    string postedBy; //will display the email of the person
};

//data structure for a simple donation of just money
//allows a user to just enter an email and amount of money to donate
struct MonetaryDonation {
    double amount;
    string fund;
    string userEmail;
};

//set up some dummy/test users to input into the system

vector<User> users = {
  //Email                  Password     Name               Role         
  //these are standard users each to a specific role            
    {"user@community.org", "UserHere", "Community Member Guy", "User"}, 
    {"admin@community.org", "Im In", "Community Admin Person", "Admin"},
    {"tester@community.org", "What?", "QA Tester or whatever", "Tester"}, 
  
//these are silly Users 
     {"CurrentlyExisting@gmail.com", "1234567890", "Ms.Baddie Bombshell", "User"},
     {"SinceYouBeenGone@hotmail.com", "I can breathe for the first time", "Kelly", "User"},
     {"IKindaFeelLikeDancing@MyGalaxy", "ThePheonixHasAwakened", "Baby-Lon", "Admin"},
     {"xXDemon-GodXx@gmail.com", "BetterThanU", "Timmy Thompson", "User"},
     {"IDontWorkHereYet@gmail.com", "Qwerty<3WASD=)", "Johnathon Lonathon", "Tester"},
     {"KK@hotmail.com", "Percussive", "Cleaner", "Admin"},
     {"SimonMan2000@hotmail.com","D to the J to the...","Simon Majimon","Admin"},
     {"Steve@Minecraft.com","HitThatBlockTillItBreak","Steve","User"},
     {"LobotoMii@LobotomYou","I feel a bit more like myself","Marie","User"},
     {"UltimateReporter@blank.com","I am the Ultimate Reporter","Evila","User"}

};


vector<Item> items = {
//  ID,Name,Category,Condition,Description,Location,Status,Postedby
    {1, "Old Wood Dining Table", "Furniture", "Good", "Oak table with visible wear. Seats 4 (Its a square).", "Somewhere Up North", "Available", "user@community.org"},
    {2, "Some winter coats", "Clothing", "Like New", "An assortment of winter jackets, sizes M/L/XL.", "North Pole", "Pending Pickup", "admin@community.org"},
    {3, "Board game set", "Toys & Games", "Fair", "Contains Connect Four, Sorry, and Monopoly (All pieces included).", "123Address", "Claimed", "tester@community.org"},
    {4, "Magical Rattle", "Toys & Games", "Good", "A magic Rattle that blew up a planet", "Baby-Lon Galaxy", "Claimed", "IKindaFeelLikeDancing@MyGalaxy"},
    {5, "Album", "Electronics", "Like New", "I didn't know she sang I thought she rapped or whatever", "THEStudio", "Pending Pickup", "SinceYouBeenGone@hotmail.com"},
    {6, "Bucket and Mop", "Other", "Fair", "I got new cleaning supplies. Maybe someone can use my old ones", "UndergroundWaterworks", "Available", "KK@hotmail.com"},
    {7, "Waxed Lightly Weathered Cut Copper Stairs x 64","Furniture", "Fair", "I crafted these for my house, but I found a prettier block to use", "The grey house with the green roof in the wooded badlands", "Available", "Steve@Minecraft.com"},
    {8, "A weak Looking Elastic Rope(How Terrifying!)","Other","like new", "I don't want to see this thing ever again...", "Yoomian Island", "Available", "LobotoMii@LobotomYou"},
    {9, "X button Beam","Other","Good", "Used to Destroy Morolians", "Channel 5 Station", "Available", "UltimateReporter@blank.com"},
    
    
};

vector<MonetaryDonation> donations;

//A global variable to track who is logged in
//-1 = guest, 1,2,3.. = User[]
// Use vector index instead of raw pointer to avoid dangling pointer issues after vector reallocation
int currentUserIndex = -1; 

//some useful functions

//clears/flushes/optimizes input the input
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//technically, strangers will meet. So let it be known, and provide a warning
void displaySafetyBanner() {
    cout << "\n=============================WARNING=============================\n";
    cout << " SAFETY REMINDER: Always meet in public, well-lit spaces for pick-up!\n";
    cout << " Never transfer funds directly to strangers. Items are meant to be free.\n";
    cout << " IF something looks suspicious or off, leave the area immediately!\n";
    cout << " Be sure to report any suspicious activity to the appropriate authorities\n";
    cout << " Items can be removed without warning if are deemed not suitable for this system\n";
    cout << "                    But other than that, Enjoy :)\n";
    cout << "===============================WARNING===========================\n";
}

//print the item with its details
void printItem(const Item& item) {
    cout << "\n[ID: " << item.id << "] " << item.title << " | Category: " << item.category << "\n";
    cout << "  Status: [" << item.status << "] | Condition: " << item.condition << "\n";
    cout << "  Description: " << item.description << "\n";
    cout << "  Pickup Location: " << item.location << " | Posted By: " << item.postedBy << "\n";
    cout << "-----------------------------------------------------------------\n";
}

//a function for navigating the menu
//when "back", "cancel", or "0" is enetered allows user to back out of a prompt
bool isCancelCommand(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return (str == "back" || str == "cancel" || str == "0");
}

//get the current user through a pointer
//returns the memory address of the active user
//returns a null pointer if there is no active user(Guest)
User* getCurrentUser() {
    if (currentUserIndex >= 0 && currentUserIndex < (int)users.size()) {
        return &users[currentUserIndex];
    }
    return nullptr;
}

//Core System Features(These will be presented to the user, and will serve as the main menu loop)

//Create User Account
void signUp() {
    cout << "\n----Create an Acount (or type 'back' to cancel)----\n";
    string email, password, name;

    clearInputBuffer();
//prompt user for name and accept
    cout << "Enter Name: ";
    getline(cin, name);
    if (isCancelCommand(name)) {
        cout << "\nRegistration canceled. Returning to main menu...\n";
        return;
    }
//prompt user for email address and accept
    cout << "Enter Email Address: ";
    getline(cin, email);
    if (isCancelCommand(email)) {
        cout << "\nRegistration canceled. Returning to main menu...\n";
        return;
    }

//Check if the email already exists
    for (const auto& u : users) {
        if (u.email == email) {
            cout << "\n[Error] An account with this email is already taken :/\n";
            return;
        }
    }
//prompt user for password and accept
    cout << "Enter Password: ";
    getline(cin, password);
    if (isCancelCommand(password)) {
        cout << "\nRegistration canceled. Returning to main menu...\n";
        return;
    }

    //Create the new user
    //default all new users to User
    //add to the list of users
    User newUser = {email, password, name, "User"};
    users.push_back(newUser);

    //Automtically log in newly created account after registration
    currentUserIndex = users.size() - 1;

    cout << "\nAccount successfully created! You are now logged in as " << users[currentUserIndex].name << ".\n";
}

//allow user to login
void login() {
    string email, password;
    cout << "\n--- LOGIN (or type 'back' to cancel) ---\n";
    
    clearInputBuffer();

    cout << "Email: ";
    getline(cin, email);
    if (isCancelCommand(email)) {
        cout << "\nLogin canceled. Returning to main menu...\n";
        return;
    }

    cout << "Password: ";
    getline(cin, password);
    if (isCancelCommand(password)) {
        cout << "\nLogin canceled. Returning to main menu...\n";
        return;
    }
//check if email and password match an existing account
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].email == email && users[i].password == password) {
            currentUserIndex = i;
            cout << "\nLogged In Successfully! Hello and Welcome, " << users[i].name << " (" << users[i].role << ").\n";
            return;
        }
    }
    cout << "\n[AN ERROR OCCURRED] Invalid email or password :/\n";
}

//allow user to browse items
void browseItems() {
    //display available options
    cout << "\n----Browse and serach for item(s)----\n";
    cout << "1. View all available Items\n";
    cout << "2. Filter by Category\n";
    cout << "0. Go Back\n";
    cout << "Choice: ";
    int choice;
    if (!(cin >> choice)) {
        clearInputBuffer();
        cout << "\nInvalid input. Returning to main menu...\n";
        return;
    }

    if (choice == 0) {
        cout << "\nReturning to main menu...\n";
        return;
    }

    //handle filter choices
    string filterCategory = "";
   //if the filter option was selected
    if (choice == 2) {
        clearInputBuffer();
        cout << "Enter Category (Furniture, Clothing, Toys & Games, Electronics, Other): ";
        getline(cin, filterCategory);
        //if the filter option was backed out of
        if (isCancelCommand(filterCategory)) {
            cout << "\nFilter canceled. Returning to main menu...\n";
            return;
        }
    }

    displaySafetyBanner();

//loop through all items and check for a match
//if found, display them 
    bool found = false;
    for (const auto& item : items) {
        if (filterCategory.empty() || item.category == filterCategory) {
            printItem(item);
            found = true;
        }
    }
//if no items found
    if (!found) {
        cout << "No items in that category found :/\n";
    }
}

//Allow user to post an item
void postItem() {
    User* user = getCurrentUser();
    if (!user) {
        cout << "\n[AN ERROR OCCURRED You must be logged in to post an item!\n";
        return;
    }

    clearInputBuffer();
    cout << "----Post an item (or type 'back' to cancel)----\n";
    cout << "----Keep it appropriate and acceptable----\n";
    cout << "----unnacepatble items will be removed----\n";
    //display details for user to fill out
    cout << "Title: ";
    string title;
    getline(cin, title);
    if (isCancelCommand(title)) {
        cout << "\nPosting canceled. Returning to main menu...\n";
        return;
    }

    //create a new Item and set up its default values
    //increase item size, set who posted it(email cureently logged in)
    //display its status(defaults to AVailable), add the title that the user just inputted
    Item newItem;
    newItem.id = items.size() + 1;
    newItem.postedBy = user->email;
    newItem.status = "Available";
    newItem.title = title;

    //continue adding the rest of the details that a user inputted
    cout << "Category (Furniture, Clothing, Toys & Games, Electronics, Other): ";
    getline(cin, newItem.category);
    if (isCancelCommand(newItem.category)) {
        cout << "\nPosting canceled. Returning to main menu...\n";
        return;
    }

    cout << "Condition (New, Like New, Good, Fair): ";
    getline(cin, newItem.condition);
    if (isCancelCommand(newItem.condition)) {
        cout << "\nPosting canceled. Returning to main menu...\n";
        return;
    }

    cout << "Description: ";
    getline(cin, newItem.description);
    if (isCancelCommand(newItem.description)) {
        cout << "\nPosting canceled. Returning to main menu...\n";
        return;
    }

    cout << "Pickup Location / Zone: ";
    getline(cin, newItem.location);
    if (isCancelCommand(newItem.location)) {
        cout << "\nPosting canceled. Returning to main menu...\n";
        return;
    }

    //add the item to the list of items
    items.push_back(newItem);
    cout << "\nSuccess! Your item listing has been published with ID #" << newItem.id << ".\n";
}

//Allow users to process a pickup
void processPickup() {
    User* user = getCurrentUser();
    if (!user) {
        cout << "\n[AN ERROR OCCURRED] You must be logged in to manage or request a pick-up.\n";
        return;
    }
//check if entered ID an already existing one.
    cout << "\nEnter Item ID to request or update pickup status (or 0 to Go Back): ";
    int targetId;
    if (!(cin >> targetId)) {
        clearInputBuffer();
        cout << "\nInvalid ID entered. Returning to main menu...\n";
        return;
    }

    if (targetId == 0) {
        cout << "\nReturning to main menu...\n";
        return;
    }
//check if the status is set to AVailable.
//only items marked as Available will be able to be taken
//set available staus to pending pickup afterwards
//display contact details after a successful claim
    for (auto& item : items) {
        if (item.id == targetId) {
            if (item.status == "Available" && item.postedBy != user->email) {
                item.status = "Pending Pickup";
                cout << "\nItem reuest success! A request has been sent to poster. The status of this item has changed to 'Pending Pickup'.\n";
                cout << "Contact details for " << item.postedBy << " have been shared with you.\n";
            }
        //handle the poster after having their item claimed
        //when a poster has their item set to pendung pickup after another user claims it
        //the poster can update the item status which will chanfe it claim
        //this emulates the poster handing over the item to the user
            else if (item.postedBy == user->email && item.status == "Pending Pickup") {
                item.status = "Claimed";
                cout << "\nItem Pickup Success! You confirmed handing over the item. Item status has been changed to 'Claimed'.\n";
            } 
           //if an item is marked as "pending pickup" or "claimed" deny the ability to take it
            else {
                cout << "\nUnable to update status. Current status: " << item.status << " As in, Its already taken :/" <<"\n";
            }
            return;
        }
    }
    //if no item is found
    cout << "\nItem ID not found.\n";
}

//handle admin moderation. Admins have the ability to take down items. 
void adminModeration() {
    
    //only allow admins to entet this area.
    //check if the user status is set to admin
    //if not, deny access
    User* user = getCurrentUser();
    if (!user || user->role != "Admin") {
        cout << "\n[ACCESS DENIED] Admin privileges required.\n";
        return;
    }

    //allow admin to take down items once logged in succesfully
    cout << "\n----Admin Moderation Menu----\n";
    cout << "Enter Item ID to remove (or 0 to Go Back): ";
    int targetId;
    if (!(cin >> targetId)) {
        clearInputBuffer();
        cout << "\nInvalid ID entered. Returning to main menu...\n";
        return;
    }

    if (targetId == 0) {
        cout << "\nReturning to main menu...\n";
        return;
    }
//scan through items and search for the item id
    auto thatsIT = remove_if(items.begin(), items.end(), [targetId](const Item& i) {
        return i.id == targetId;
    });

    //check if teh item was actually found
if (thatsIT != items.end()) {
    //delete it from the list    
    items.erase(thatsIT, items.end());
        cout << "\nListing #" << targetId << " has been successfully removed by Admin moderation.\n";
    } 
    //if no item has been found
    else {
        cout << "\nItem ID not found.\n";
    }
}

//Allow users to donate money
void monetarySupport() {
    cout << "\n----Monetary Support. Would you like to donate?----\n";

    double amount;
    string fund;

//prompt user to donate any said amount, but specify that its in dollars. 
 cout << "Enter Donation Amount ($ USD, or 0 to Go Back): ";
   
 //check that only numbers in the double form have been entered, and not other characters
 if (!(cin >> amount)) {
        clearInputBuffer();
        cout << "\nInvalid amount. Returning to main menu...\n";
        return;
    }

         if (amount == 0) {
        cout << "\nReturning to main menu...\n";
        return;
    }

    clearInputBuffer();
   //allow the user to select an organization to donate to
    cout << "Select Fund (Logistics, Youth Programs, General Support): ";
    getline(cin, fund);
    if (isCancelCommand(fund)) {
        cout << "\nTransaction canceled. Returning to main menu...\n";
        return;
    }
//get the user who is currently using program
    User* user = getCurrentUser();
    //create an instance of the monetaryDonation struct to hold the donatation details
    MonetaryDonation donatedDetails;
    donatedDetails.amount = amount;
    donatedDetails.fund = fund;

    //check who is logged in. Either grab the email of a logged in user, or default to an anonymous Guest
    //because apparently, guests can donate 
    donatedDetails.userEmail = user ? user->email : "Guest";

    donations.push_back(donatedDetails);

//display output 
cout << fixed << setprecision(2); //display 2 decimal points, emulating cents
cout << "\nThank you! Successfully processed a donation in the amount of$ " << amount << " to " << fund << "'.\n";
}

//Main Menu loop.
//Apply all previously created functions here
//display a main menu with all options that users can interact with
//typing in "back", "cancel", or "o" can be used to back out of an option
//use a switch case to carry out the operation

int main() {
    //choice variable for the user. Starts at 0 since 0 isnt an option.
    int choice = 0;

    //9 breaks out of the loop and stops the program. Continue looping until user exits
    while (choice != 9) {
        //grab the active user/user currently logged in
        User* activeUser = getCurrentUser();

     //display main menu to user
        cout << "\n=================================================================\n";
        cout << "                COMMUNITY DONATION SYSTEM \n";
        //print out the name of whoever is logged in
        if (activeUser) {
            cout << " Active User: " << activeUser->name << " [" << activeUser->role << "]\n";
        } 
        //if no one is logged in, default to Guest
        else {
            cout << " Active User: Guest (Not Logged In)\n";
        }

        //display a list of options to User with the numeber that correcsponds to that option
        //this will be switch out in a switch case
        cout << "=================================================================\n";
        cout << "1. Login\n";
        cout << "2. Sign Up (Create Account)\n";
        cout << "3. Browse & Search Available Items\n";
        cout << "4. Post an Item\n";
        cout << "5. Request Pickup / Update Pickup Status\n";
        cout << "6. Support Community Programs (Monetary Support)\n";
        cout << "7. Admin Moderation (Remove Listing) [ADMINS ONLY]\n";
        cout << "8. Logout\n";
        cout << "9. Exit\n";
        cout << "Select Option (1-9): ";


    //check if invalid characters have been selected. (Anythng other than 1-9)
        if (!(cin >> choice)) {
            cout << "\nInvalid input. Please enter a number from 1 to 9.\n";
            clearInputBuffer();
            choice = 0; // reset choice
            continue; // carry on
        }
//switch statement where the actual function occurrs and activates.
//Once a user selects an option that function will switch with it and activate 
        switch (choice) {
            case 1: login(); break;
            case 2: signUp(); break;
            case 3: browseItems(); break;          
            case 4: postItem(); break;
            case 5: processPickup(); break;
            case 6: monetarySupport(); break;
            case 7: adminModeration(); break;
         
         //log out user by setting Current User Index back to -1, meaning no one is currently logged in   
            case 8: 
                currentUserIndex = -1; 
                cout << "\nSuccessfully Logged out.\n"; 
                break;
         
        //Exit out the loop when 9 is selected. 
        //loop condition becomes alse and terminates loop        
            case 9: 
                cout << "\nExiting application. Come back another time, and have a good day :)\n"; 
                break;
         
        //default case to catch all other inputs 
            default: 
                cout << "\nInvalid selection. Please choose an option between 1 and 9.\n"; 
                break;
        }
    }

    return 0;
}


