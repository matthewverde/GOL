
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// In order to load the resources like cute_image.png, you have to set up
// your target scheme:
//
// - Select "Edit Scheme…" in the "Product" menu;
// - Check the box "use custom working directory";
// - Fill the text field with the folder path containing your resources;
//        (e.g. your project folder)
// - Click OK.
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "board.h"
#include "creature.h"

using namespace std;
using namespace sf;

vector<RectangleShape> drawCreature(creature bestCreature, int boardSize)
{
    vector<RectangleShape> toRet;
    float rectSize = 1500 / boardSize;
    for(int i = 0; i < bestCreature.creatureLocation.size(); i++)
    {
        RectangleShape newShape;
        newShape.setSize(Vector2f(rectSize,rectSize));
        newShape.setFillColor(Color::White);
        newShape.setPosition(bestCreature.creatureLocation[i].x * rectSize, bestCreature.creatureLocation[i].y * rectSize);
        toRet.push_back(newShape);
    }
    
    return toRet;
}

vector<RectangleShape> drawBoard(int boardSize, board myBoard)
{
    vector<RectangleShape> toRet;
    float rectSize = 1500 / boardSize;
    for(int row = 0; row < myBoard.boardSize; row++)
    {
        for(int col = 0; col < myBoard.boardSize; col++)
        {
            if(myBoard.curBoard[row][col])
            {
                RectangleShape newShape;
                newShape.setSize(Vector2f(rectSize,rectSize));
                newShape.setFillColor(Color::White);
                newShape.setPosition(col * rectSize, row * rectSize);
                toRet.push_back(newShape);
            }
        }
    }
    
    RectangleShape newShape;
    newShape.setSize(Vector2f(rectSize,rectSize));
    newShape.setFillColor(Color::Red);
    newShape.setPosition(myBoard.goalLocation.x * rectSize, myBoard.goalLocation.y * rectSize);
    toRet.push_back(newShape);
    
    return toRet;
}

int main(int argc, char const** argv)
{
    // Create the main window
    int boardSize = 50;
    sf::RenderWindow window(sf::VideoMode(1500, 1500), "SFML window");
    board myBoard(boardSize, Vector2i(40,40));
    vector<RectangleShape> toDraw;
    unsigned short score;
    bool first = true;
    
    srand(time(NULL));
    myBoard.runEvolution(1000, 100);
    creature bestCreature = myBoard.curCreature;
    
    toDraw = drawCreature(bestCreature, boardSize);
    
    //cout << "Size: " << toDraw.size() << endl;
    
    myBoard.resetBoard();
    myBoard.setBoard();

    sf::Time e1;
    sf::Clock clock;

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        e1 = clock.getElapsedTime();
        if(first && e1.asMilliseconds() < 5000)
        {
            // Clear screen
            window.clear();
            
            toDraw = drawBoard(boardSize, myBoard);
            
            for(auto shape : toDraw)
            {
                window.draw(shape);
            }
            
            window.display();
        }
        else if(first)
        {
            first = false;
            clock.restart();
        }
        else if(e1.asMilliseconds() > 200)
        {
            // Clear screen
            window.clear();
            myBoard.simulate(score);
        
            toDraw = drawBoard(boardSize, myBoard);

            for(auto shape : toDraw)
            {
                window.draw(shape);
            }
            clock.restart();
            window.display();
        }
        

        // Update the window
        
    }

    return EXIT_SUCCESS;
}
