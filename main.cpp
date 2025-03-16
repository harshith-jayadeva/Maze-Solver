/*CSCI 200: Assignment 6: A6 - Maze Finder
 *
 * Author: XXXX (Harshith Jayadeva)
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     // list here any outside assistance you used/received while following the
 *     // CS@Mines Collaboration Policy and the Mines Academic Code of Honor
 * 
 *     Recieved help from professor Scholten in animating the squares. I had to use an if statement instead of a while loop
 * 
 * Description -> Used BFS and DFS to find a path to the end of the maze. To backtrack in DFS, I used a stack to add previously visited rectangles, 
 * If I reach a dead end, then the program backtracks until a rectangle with a path forwards is found. 
 * 
 * I used a queue for BFS and added all the neighbors of the current rectangle to the queue. Repeated this process of add all neighbors to the queue
 * and checking them until I reached the end or determined there was no path found. 
 * */
#include <fstream>
#include <iostream>
#include <stack>
#include <queue>
#include <string>

#include "Rectangle.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

int main(int argc, char *argv[]){
    // Using command line arguments to get filename
    std::string filename;
    if ( argc != 2 ){
        std::cout<<"usage: "<< argv[0] <<" <filename>\n";
        std::cout<<"Enter filename here: ";
        std::cin >> filename;
    }

    
    // We assume argv[1] is a filename to open
    std::ifstream myFile(filename);
    // Always check to see if file opening succeeded
    if ( !myFile.is_open() ){
        std::cout<<"Could not open file\n";
    }
    else{
        // Reading in the Rows and Columns in the maze
        int r, c;
        char character;
        myFile >> r >> c;

        // Declaring the window
        sf::RenderWindow window(sf::VideoMode(c * 15, r * 15), "A6");

        // Event objects for future events
        sf::Event event;
        sf::Clock clock;

        // Declaring the starting and ending points of the maze
        int start_x, start_y, end_x, end_y;

        // Dynamically allocating the arrays that will store the information about the maze and which spots we have visited.
        Rectangle*** arr = new Rectangle**[r];
        for(int i = 0; i < r; i++){
            arr[i] = new Rectangle*[c];
        }

        // Dynamically allocating the arrays that will store the information about the maze and which spots we have visited.
        bool** visited = new bool*[r];
        for(int i = 0; i < r; i++){
            visited[i] = new bool[c];
        }

        // Dynamically allocating the arrays that will store the information about the maze and which spots we have visited.
        char** map = new char*[r];
        for(int i = 0; i < r; i++){
            map[i] = new char[c];
        }

        // Reading the file and instanciating the correct rectangle objects. 
        for (int rows = 0; rows < r; rows++) {
            for (int cols = 0; cols < c; cols++) {
                // Populating the matrices
                myFile >> character;
                map[rows][cols] = character;
                visited[rows][cols] = false;

                // Creating a starting rectangle if S is read from file
                if (character == 'S') {
                    arr[rows][cols] = new Rectangle(sf::Color::Green, cols, rows);
                    start_x = rows;
                    start_y = cols;
                }

                // Creating a ending rectangle if a E is read from the file
                if (character == 'E') {
                    arr[rows][cols] = new Rectangle(sf::Color::Red, cols, rows);
                    end_x = rows;
                    end_y = cols;
                }

                // Creating a wall rectangle if a # is read from the file
                if (character == '#') {
                    arr[rows][cols] = new Rectangle(sf::Color::Black, cols, rows);
                }

                // Creating a rectangle that will represent our path if a . is read from the file
                if (character == '.') {
                    arr[rows][cols] = new Rectangle(sf::Color::White, cols, rows);
                }
            }
        }

        // Asking the user if they would like to perform a DFS or a BFS
        std::string choice;
        std::cout << "Would you like to perform a [BFS] Breadth First Search or a [DFS] Depth First Search?" << std::endl;
        std::cin >> choice;

        // Declaring the stack and queue to be used in the search algorithms
        std::stack<Rectangle*> positions; 
        std::queue<Rectangle*> q_positions;

        // Creating 2 arrays that will allow us to move up down left right in the maze. 
        int x[4] = {0, 1, 0, -1};
        int y[4] = {1, 0, -1, 0};
        
        // Pushing the first starting rectangle onto the stack
        // Declaring the current positions
        int curr_x = start_x; 
        int curr_y = start_y; 
        int new_x, new_y;
        bool moved = false;
        positions.push(arr[start_x][start_y]);
        q_positions.push(arr[start_x][start_y]);

        // DFS algorithm
        window.display();
        while(window.isOpen()) {
            if(choice == "DFS" || choice == "dfs"){
                // Runs until stack is empty which either means path was found or no path was found
                if(!positions.empty()){
                    moved = false;

                    // Checks to see if predefined movements are valid in the maze. 
                    for(int i = 0; i < 4; i++){
                        new_x = curr_x + x[i];
                        new_y = curr_y + y[i];

                        // If a movement is valid, the program marks this new spot as visited and adds the corresponding rectangle to the stack
                        if((new_x >= 0 && new_x < r) && (new_y >= 0 && new_y < c) && (map[new_x][new_y] == '.' || map[new_x][new_y] == 'E') && !visited[new_x][new_y]){
                            // Setting neighbors that need to be checked as blue rectangles
                            if(arr[new_x][new_y]->getColor() != sf::Color::Red){
                                arr[new_x][new_y]->setColor(sf::Color::Blue);
                            }
                            
                            // Setting the path as yellow rectangles. 
                            if(!moved){
                                curr_x = new_x;
                                curr_y = new_y;
                                visited[curr_x][curr_y] = true;
                                positions.push(arr[curr_x][curr_y]);
                                arr[curr_x][curr_y]->setColor(sf::Color::Yellow);
                                moved = true;
                            }
                        }
                    }

                    // If the program wasn't able to move, then it backtracks to the previous rectangle to see if any valid movements can be performed on that rectangle. 
                    if(!moved){
                        arr[curr_x][curr_y]->setColor(sf::Color::Magenta);
                        positions.pop();
                        // If that rectangle does have valid movements, then the current x and y coordinate are set to that rectangle
                        if(!positions.empty()){
                            curr_x = positions.top()->_yCoord;
                            curr_y = positions.top()->_xCoord;
                        }  
                    }
                    
                    // Program exits the loop if it reaches the end x and y values
                    if (curr_x == end_x && curr_y == end_y){
                        arr[curr_x][curr_y]->setColor(sf::Color::Red);
                        std::cout<< "Reached the end!" << std::endl;
                        sf::sleep(sf::seconds(5));
                        break;
                    }
                
                    window.clear();
                    // Drawing the rectangles to the window. 
                    for(int rows = 0; rows < r; rows++){
                        for(int cols = 0; cols < c; cols++){
                            arr[rows][cols]->draw(window);
                            
                        }
                    }
                    
                    window.display();
                    sf::sleep(sf::milliseconds(50));
                }

                // If the previous if loop wasn't broken, then path was not found
                else{
                    std::cout << "Could not find a path to the end" << std::endl;
                    sf::sleep(sf::seconds(5));
                }

            }

            // BFS Algorithm
            if(choice == "BFS" || choice == "bfs"){
                // Starting rectangle is pushed onto the queue

                if(!q_positions.empty()) {
                    // We get the rectangle stored in the queue and assign it to our current rectangle and then pop it from the queue. 
                    Rectangle* current_rectangle = q_positions.front();

                    q_positions.pop();
                    curr_x = current_rectangle->_yCoord;
                    curr_y = current_rectangle->_xCoord;

                    // Program exits the loop if it reaches the end x and y values
                    if (map[curr_x][curr_y] == 'E') {
                        std::cout<< "Reached the end!" << std::endl;
                        sf::sleep(sf::seconds(5));
                        break;
                    }

                    // Checks to see if the current rectangle has any valid movmements. 
                    for (int i = 0; i < 4; i++) {
                        new_x = curr_x + x[i];
                        new_y = curr_y + y[i];

                        // If the rectangle has any valid movements, it is appended to the queue. 
                        if ((new_x >= 0 && new_x < r) && (new_y >= 0 && new_y < c) && (map[new_x][new_y] == '.' || map[new_x][new_y] == 'E') && !visited[new_x][new_y]) {
                            
                            // We mark the current rectangle as visited
                            visited[new_x][new_y] = true;

                            // Unlike DFS, we don't break from the while loop once we have found a valid move, we add all rectangles we reach from valid movements into the queue. 
                            q_positions.push(arr[new_x][new_y]);
                            if(arr[new_x][new_y]->getColor() != sf::Color::Red){
                                arr[new_x][new_y]->setColor(sf::Color::Magenta);
                            }
                        }
                    }

                    window.clear();
                    // Drawing the rectangles to the window. 
                    for(int rows = 0; rows < r; rows++){
                        for(int cols = 0; cols < c; cols++){
                            arr[rows][cols]->draw(window);
                        }
                    }
                    
                    window.display();
                    sf::sleep(sf::milliseconds(50));

                }
                 // If the previous if loop wasn't broken, then path was not found
                else{
                    std::cout << "Could not find a path to the end" << std::endl;
                    sf::sleep(sf::seconds(5));
                }
            }
            
            // SFML loop  
            // While loop that checks for user input
            while(window.pollEvent(event)) {
                // If event type corresponds to pressing window close button, q key or escape key
                if(event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Q || event.key.code == sf::Keyboard::Escape) {
                    // Closes the window
                    window.close();
                }
            }
        }
    }
    return 0;
}

