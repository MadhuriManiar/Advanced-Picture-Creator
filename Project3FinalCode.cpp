//Madhuri Maniar
//MVM190002
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

//create structure
struct node{
    char var = ' ';
    node *up = nullptr;
    node *down = nullptr;
    node *left = nullptr;
    node *right = nullptr;
};

node* createGrid(int, int);
void printGrid(node *curr, node *temp);
void printOutfile(node *curr, node *temp, ofstream&);
void deleteGrid(node *Grid);
void drawNorth(bool, int, const string&, const string&, node *&current);
void drawSouth(bool, int, const string&, const string&, node *&current);
void drawEast(bool, int, const string&, const string&, node *&current);
void drawWest(bool, int, const string&, const string&, node *&current);

int main() {
    int row = 50;
    int col = 50;
    string input;
    //prompt user for name of the input file
    cout << "Enter name of the input file. " << endl;
    //reads the user input and opens that file
    getline(cin, input);
    //opens text file
    ifstream infile(input);
    ofstream outfile("paint.txt", ios::out);
    //outfile.open("paint.txt", ios::in | ios::out);
    //if the file does not exist
    if (infile.fail())
        cout << "Error in opening file. " << endl;
        //if the file does exist
    else if (infile) {
        string pos1, pos2, pos3, pos4, pos5;
        string line, penmode, bold, print, direction, IgnoreLine;
        int distance, posNS = 0, posEW = 0, NewposNS, NewposEW;
        bool pendown;
        node *Grid;
        Grid = createGrid(row, col);
        node *current = Grid;
        node *curr = Grid;
        node *temp = Grid;
        while (getline(infile, line)) {
            //splitting up each character in each line of the file using the comma as a delimiter
            stringstream linestream(line);
            getline(linestream, pos1, ',');
            getline(linestream, pos2, ',');
            getline(linestream, pos3, ',');
            getline(linestream, pos4, ',');
            getline(linestream, pos5, ',');

            //assigns variables to each position
            penmode = pos1;
            direction = pos2;
            //checks to make sure the distance is an int
            try{
                distance=stoi(pos3);
            }
            catch (invalid_argument& e) {
                continue;
            }
            //checks to make sure the distance is not a negative
            if(distance<0)
                continue;
            //checks for an empty space instead of a char
            for (int i = 0; i < (int)line.length(); ++i) {
                if (line[i] == ' ')
                    continue;
            }
            //checks for extra spaces in the code
            int index = (int)line.find(' ');
            if (index>0)
                continue;
            //checks for extra commas
            index = (int)line.find(",,");
            if (index>0)
                continue;
            //checks for no commas
            index = (int)line.find(',');
            if (index==0)
                continue;
            //checks for extra comma after commands
            if (line.back()== ',')
                continue;

            //penmode
            //if the penmode is 1, the pen is up; no print
            if (penmode == "1")
                pendown = false;
                //if the penmode is 2, the pen is down; yes print
            else if (penmode == "2")
                pendown = true;
                //if the penmode isn't one or two
            else
                continue;

            //checks if there is a B or P in the command based on the length of command
            if (line.length() >= 5 && line.length() <= 6) {
                pos4 = "null";
                pos5 = "null";
            }
            //checks to make sure the commands is the correct length
            if (line.length()<5 || line.length()>10)
                continue;

            //checks to make sure the pen is not up and bold and the same time
            if (penmode=="1" && pos4=="B")
                continue;
            //checks for extra P's
            if(pos4=="P"&&pos5=="P")
                continue;
            //checks for flipped bold and print commands
            if (pos4=="P"&&pos5=="B")
                continue;
            //check for lowercase print commands
            if (pos4=="p" || pos4=="b" || pos5=="p" || pos5=="b")
                continue;


            //for direction north
            if (direction == "N") {
                //checks to make sure the pointer has gone too north and out of the canvas
                NewposNS = posNS - distance;
                if (NewposNS>49 || NewposNS<0)
                {
                    continue;
                }
                else {
                    //when the pointer is inside the bounds, execute
                    drawNorth(pendown, distance, pos4, pos5, current);
                    posNS = NewposNS;
                }
            }
                //for direction south
            else if (direction == "S") {
                //checks to make sure the pointer has gone too south and out of the canvas
                NewposNS = posNS + distance;
                if (NewposNS>49 || NewposNS<0)
                {
                    continue;
                }
                else {
                    //when the pointer is inside the bounds, execute
                    drawSouth(pendown, distance, pos4, pos5, current);
                    posNS = NewposNS;
                }
            }
                //for direction east
            else if (direction == "E") {
                //checks to make sure the pointer has gone too east and out of the canvas
                NewposEW = posEW + distance;
                if (NewposEW>49 || NewposEW<0)
                {
                    continue;
                }
                else {
                    //when the pointer is inside the bounds, execute
                    drawEast(pendown, distance, pos4, pos5, current);
                    posEW = NewposEW;
                }
            }
                //for direction west
            else if (direction == "W") {
                //checks to make sure the pointer has gone too west and out of the canvas
                NewposEW = posEW - distance;
                if (NewposEW>49 || NewposEW<0)
                {
                    continue;
                }
                else {
                    //when the pointer is inside the bounds, execute
                    drawWest(pendown, distance, pos4, pos5, current);
                    posEW = NewposEW;
                }
            }
            else {
                continue;
            }
            //prints out to console every time print is called
            if (pos4 == "P" || pos5 == "P") {
                printGrid(curr, temp);
                cout<<endl;
            }
        }
        //print to output file after input file is at the end
        if (infile.eof())
            printOutfile(curr, temp, outfile);
        //close files
        infile.close();
        outfile.close();
        deleteGrid(Grid);
    }
}


node* createGrid(int rows, int cols)
{
    //create the grid and connect the nodes using the pointers
    node *prev = new node;
    node *above = new node;
    node *n;
    node *head;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            //add node
            n = new node;
            if (i==0 && j==0) {
                head = n;
            }

            else if (i==0) {
                prev->right = n;
                n->left = prev;
            }
            else{
                if (j==0){
                    above = head;
                    if (i!=1) {
                        for (int k = 0; k < i - 1; k++) {
                            above = above->down;
                        }
                    }
                }
                n->up = above;
                above->down = n;
                if (j!=0) {
                    n->left = prev;
                    prev->right = n;
                }
                above = above->right;
            }
            prev = n;
        }
    }
    return head;
}

void printGrid(node *curr, node *temp)
{
    //iterate through the grid and print to the console
    if (curr == nullptr)
        return;
    cout<<curr->var;
    if (curr->right == nullptr && curr->down != nullptr) {
        cout << endl;
        for (int i = 0; i < 49; i++) {
            curr = curr->left;
        }
        curr = curr->down;
        printGrid(curr, temp);
    }
    else if (curr->right != nullptr)
        printGrid(curr->right, temp);

}

void printOutfile(node *curr, node *temp, ofstream& outfile)
{
    //iterate through the grid and print to the outfile
    if (curr != nullptr) {
        outfile << curr->var;
    }
    if (curr->right != nullptr){
        printOutfile(curr->right, temp, outfile);
    }
    if (curr->right == nullptr && curr->down != nullptr){
        outfile<<endl;
        temp = temp->down;
        curr = temp;
        if (curr != nullptr){
            printOutfile(curr, temp, outfile);
        }
    }
}

void deleteGrid(node *Grid)
{
    //delete nodes of the grid
    node *rightp= Grid;
    node *downp = Grid;
    while (downp != nullptr){
        while (rightp != nullptr){
            delete rightp;
            rightp = rightp->right;
        }
        downp = downp->down;
    }
}

//if the direction is north
void drawNorth(bool pendown, int distance, const string& pos4, const string& pos5, node *&current) {
    //when the pen is up, nothing prints and the pointer is moved north an amount of times
    if (!pendown) {
        for (int i = 0; i < distance; ++i){
            current = current->up;
        }
    }
    //when the pen is down, a character prints out moving north
    if (pendown) {
        if (distance == 0){
            return;
        }
        current = current->up;
        for(int j = 0; j<distance; j++) {
            //when both P and B are in the command
            if (pos5 == "P" && pos4 == "B") {
                current->var = '#';
            }
                //when only B is in the command
            else if (pos4 == "B") {
                current->var = '#';
            }
            else if (current->var == '#')
                current->var = '#';
                //when only P is in the command
            else if (pos4 == "null" || pos4 == "P") {
                current->var = '*';
            }
            if (j != distance-1)
                current = current->up;
        }
    }
}

//if the direction is south
void drawSouth(bool pendown, int distance, const string& pos4, const string& pos5, node *&current)
{
    //when the pen is up, nothing prints and the pointer is moved south an amount of times
    if (!pendown){
        for (int i = 0; i < distance; ++i)
        {
            current = current->down;
        }
    }
    //when the pen is down, a character prints out moving south
    if (pendown){
        if (distance == 0){
            //when both P and B are in the command
            if (pos5 == "P" && pos4 == "B") {
                current->var = '#';
            }
                //when only B is in the command
            else if (pos4 == "B") {
                current->var = '#';
            }
                //when only P is in the command
            else if (pos4 == "null" || pos4 == "P") {
                current->var = '*';
            }
        }
        current = current->down;
        for (int j = 0; j < distance; ++j) {
            //when both P and B are in the command
            if (pos5 == "P" && pos4 == "B") {
                current->var = '#';
            }
                //when only B is in the command
            else if (pos4 == "B") {
                current->var = '#';
            }
                //when only P is in the command
            else if (pos4 == "null" || pos4 == "P") {
                current->var = '*';
            }
            if (j != distance-1)
                current = current->down;
        }
    }
}

//if the direction is east
void drawEast(bool pendown, int distance, const string& pos4, const string& pos5, node *&current)
{
    //when the pen is up, nothing prints and the pointer is moved east an amount of times
    if (!pendown){
        for (int i = 0; i < distance; ++i){
            current = current->right;
        }
    }
    //when the pen is down, a character prints out moving east
    if (pendown){
        if (distance == 0){
            //when both P and B are in the command
            if (pos5 == "P" && pos4 == "B") {
                current->var = '#';
            }
                //when only B is in the command
            else if (pos4 == "B") {
                current->var = '#';
            }
                //when only P is in the command
            else if (pos4 == "null" || pos4 == "P") {
                current->var = '*';
            }
            else
                current->var = '*';
        }
        current = current->right;
        for (int i = 0; i < distance; i++){
            //when both P and B are in the command
            if (pos5 == "P" && pos4 == "B") {
                current->var = '#';
            }
                //when only B is in the command
            else if (pos4 == "B") {
                current->var = '#';
            }
                //when only P is in the command
            else if (pos4 == "null"||pos4=="P") {
                current->var = '*';
            }
            if (i != distance-1)
                current = current->right;

        }
    }
}

//if the direction is west
void drawWest(bool pendown, int distance, const string& pos4, const string& pos5, node *&current)
{
    //when the pen is up, nothing prints and the pointer is moved west an amount of times
    if (!pendown){
        for (int i = 0; i < distance; ++i)
        {
            current = current->left;
        }
    }
    //when the pen is down, a character prints out moving west
    if (pendown){
        if (distance == 0){
            //when both P and B are in the command
            if (pos5 == "P" && pos4 == "B") {
                current->var = '#';
            }
                //when only B is in the command
            else if (pos4 == "B") {
                current->var = '#';
            }
                //when only P is in the command
            else if (pos4 == "null" || pos4 == "P") {
                current->var = '*';
            }
        }
        current = current->left;
        for (int i = 0; i < distance; i++){
            //when both P and B are in the command
            if (pos5 == "P" && pos4 == "B") {
                current->var = '#';
            }
                //when only B is in the command
            else if (pos4 == "B") {
                current->var = '#';
            }
                //when only P is in the command
            else if (pos4 == "null"||pos4=="P") {
                current->var = '*';
            }
            if (i != distance-1)
                current = current->left;
        }
    }
}




